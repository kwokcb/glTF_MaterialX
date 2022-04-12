
#include <MaterialXCore/Value.h>
#include <MaterialXCore/Types.h>
#include <MaterialXFormat/XmlIo.h>
#include <MaterialXFormat/Util.h>
#include "CgltfMaterialLoader.h"

#if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wswitch"
#endif

#if defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4996)
#endif

//#define CGLTF_IMPLEMENTATION -- don't set to avoid duplicate symbols
#include <MaterialXRender/External/Cgltf/cgltf.h>

#if defined(_MSC_VER)
    #pragma warning(pop)
#endif

#if defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif

#include <cstring>
#include <iostream>
#include <limits>

MATERIALX_NAMESPACE_BEGIN

namespace mx = MaterialX;

bool CgltfMaterialLoader::load(const FilePath& filePath)
{
    const std::string input_filename = filePath.asString();
    const std::string ext = stringToLower(filePath.getExtension());
    const std::string BINARY_EXTENSION = "glb";
    const std::string ASCII_EXTENSION = "gltf";
    if (ext != BINARY_EXTENSION && ext != ASCII_EXTENSION)
    {
        return false;
    }

    cgltf_options options;
    std::memset(&options, 0, sizeof(options));
    cgltf_data* data = nullptr;

    // Read file
    cgltf_result result = cgltf_parse_file(&options, input_filename.c_str(), &data);
    if (result != cgltf_result_success)
    {
        return false;
    }
    if (cgltf_load_buffers(&options, data, input_filename.c_str()) != cgltf_result_success)
    {
        return false;
    }

    loadMaterials(data);

    if (data)
    {
        cgltf_free(data);
    }

    return true;
}

// Utilities
namespace
{
NodePtr createTexture(DocumentPtr& doc, const std::string & nodeName, const std::string& fileName,
                    const std::string & textureType, const std::string & colorspace)
{
    std::string newTextureName = doc->createValidChildName(nodeName);
    NodePtr newTexture = doc->addNode("tiledimage", newTextureName, textureType);
    newTexture->setAttribute("nodedef", "ND_image_" + textureType);
    newTexture->addInputsFromNodeDef();
    InputPtr fileInput = newTexture->getInput("file");
    fileInput->setValue(fileName, "filename");
    if (!colorspace.empty())
    {
        fileInput->setAttribute("colorspace", colorspace);
    }
    return newTexture;
}

const std::string SPACE_STRING = " ";

void setColorInput(DocumentPtr materials, NodePtr shaderNode, const std::string& inputName, 
                   const Color3& colorFactor, const cgltf_texture_view* textureView,
                   const std::string& inputImageNodeName)
{
    ValuePtr color3Value = Value::createValue<Color3>(colorFactor);
    InputPtr colorInput = shaderNode->getInput(inputName);
    if (colorInput)
    {
        cgltf_texture* texture = textureView ? textureView->texture : nullptr;
        if (texture && texture->image)
        {
            std::string imageNodeName = texture->image->name ? texture->image->name : inputImageNodeName;
            imageNodeName = materials->createValidChildName(imageNodeName);
            std::string uri = texture->image->uri ? texture->image->uri : SPACE_STRING;
            NodePtr newTexture = createTexture(materials, imageNodeName, uri,
                "color3", "srgb_texture");
            colorInput->setAttribute("nodename", newTexture->getName());
        }
        else
        {
            colorInput->setValueString(color3Value->getValueString());
        }
    }
}

void setFloatInput(DocumentPtr materials, NodePtr shaderNode, const std::string& inputName, 
                   float floatFactor, const cgltf_texture_view* textureView,
                   const std::string& inputImageNodeName)
{
    InputPtr floatInput = shaderNode->getInput(inputName);
    if (floatInput)
    {
        cgltf_texture* texture = textureView ? textureView->texture : nullptr;
        if (texture && texture->image)
        {
            std::string imageNodeName = texture->image->name ? texture->image->name :
                "image_sheen_roughness";
            imageNodeName = materials->createValidChildName(inputImageNodeName);
            std::string uri = texture->image->uri ? texture->image->uri : SPACE_STRING;
            NodePtr newTexture = createTexture(materials, imageNodeName, uri,
                "float", EMPTY_STRING);
            floatInput->setAttribute("nodename", newTexture->getName());
        }
        else
        {
            floatInput->setValue<float>(floatFactor);
        }
    }
}

}

void CgltfMaterialLoader::loadMaterials(void *vdata)
{
    cgltf_data* data = static_cast<cgltf_data*>(vdata);

    // Scan materials
    /*
    * typedef struct cgltf_material
    {
	    char* name;
	    cgltf_bool has_pbr_metallic_roughness;
	    cgltf_bool has_pbr_specular_glossiness;
	    cgltf_bool has_clearcoat;
	    cgltf_bool has_transmission;
	    cgltf_bool has_volume;
	    cgltf_bool has_ior;
	    cgltf_bool has_specular;
	    cgltf_bool has_sheen;
	    cgltf_bool has_emissive_strength;
	    cgltf_pbr_metallic_roughness pbr_metallic_roughness;
	    cgltf_pbr_specular_glossiness pbr_specular_glossiness;
	    cgltf_clearcoat clearcoat;
	    cgltf_ior ior;
	    cgltf_specular specular;
	    cgltf_sheen sheen;
	    cgltf_transmission transmission;
	    cgltf_volume volume;
	    cgltf_emissive_strength emissive_strength;
	    cgltf_texture_view normal_texture;
	    cgltf_texture_view occlusion_texture;
	    cgltf_texture_view emissive_texture;
	    cgltf_float emissive_factor[3];
	    cgltf_alpha_mode alpha_mode;
	    cgltf_float alpha_cutoff;
	    cgltf_bool double_sided;
	    cgltf_bool unlit;
	    cgltf_extras extras;
	    cgltf_size extensions_count;
	    cgltf_extension* extensions;
    } cgltf_material;
    */
    if (data->materials_count)
    {
        _materials = Document::createDocument<Document>();
        _materials->importLibrary(_definitions);
    }
    size_t materialId = 0;
    const std::string SHADER_PREFIX = "Shader_";
    const std::string MATERIAL_PREFIX = "MATERIAL_";
    for (size_t m = 0; m < data->materials_count; m++)
    {
        cgltf_material* material = &(data->materials[m]);
        if (!material)
        {
            continue;
        }

        // Create a default gltf_pbr node
        std::string matName = material->name ? material->name : EMPTY_STRING;
        if (!matName.empty() && std::isdigit(matName[0]))
        {
            matName = SHADER_PREFIX + matName;
        }
        std::string shaderName = matName.empty() ? SHADER_PREFIX + std::to_string(materialId) : matName;
        shaderName = _materials->createValidChildName(shaderName);
        NodePtr shaderNode = _materials->addNode("gltf_pbr", shaderName, "surfaceshader");
        shaderNode->setAttribute("nodedef", "ND_gltf_pbr_surfaceshader");
        shaderNode->addInputsFromNodeDef();

        // Create a surface material for the shader node
        std::string materialName = matName.empty() ? MATERIAL_PREFIX + std::to_string(materialId) : MATERIAL_PREFIX + matName;
        materialName = _materials->createValidChildName(materialName);
        NodePtr materialNode = _materials->addNode("surfacematerial", materialName, "material");
        InputPtr shaderInput = materialNode->addInput("surfaceshader", "surfaceshader");
        shaderInput->setAttribute("nodename", shaderNode->getName());

        if (material->has_pbr_metallic_roughness)
        {
            cgltf_pbr_metallic_roughness& roughness = material->pbr_metallic_roughness;

            Color3 colorFactor(roughness.base_color_factor[0],
                roughness.base_color_factor[1],
                roughness.base_color_factor[2]);
            setColorInput(_materials, shaderNode, "base_color",
                colorFactor, &roughness.base_color_texture, "image_basecolor");


            // Alpha
            InputPtr alphaInput = shaderNode->getInput("alpha");
            if (alphaInput)
            {
                alphaInput->setValue<float>(roughness.base_color_factor[3]);
            }

            // Set metalic, roughness, and occlusion
            InputPtr metallicInput = shaderNode->getInput("metallic");
            InputPtr roughnessInput = shaderNode->getInput("roughness");
            InputPtr occlusionInput = shaderNode->getInput("occlusion");

            // Check for occlusion/metallic/roughness texture
            cgltf_texture_view& textureView = roughness.metallic_roughness_texture;
            cgltf_texture* texture = textureView.texture;
            if (texture && texture->image)
            {
                std::string imageNodeName = texture->image->name ? texture->image->name :
                    "image_orm";
                imageNodeName = _materials->createValidChildName(imageNodeName);
                std::string uri = texture->image->uri ? texture->image->uri : SPACE_STRING;
                NodePtr textureNode = createTexture(_materials, imageNodeName, uri,
                    "vector3", EMPTY_STRING);

                // Add extraction nodes. Note that order matters
                StringVec extractNames =
                {
                        _materials->createValidChildName("extract_occlusion"),
                        _materials->createValidChildName("extract_roughness"),
                        _materials->createValidChildName("extract_metallic")
                };
                std::vector<InputPtr> inputs =
                {
                    occlusionInput, roughnessInput, metallicInput
                };
                for (size_t i = 0; i < extractNames.size(); i++)
                {
                    NodePtr extractNode = _materials->addNode("extract", extractNames[i], "float");
                    extractNode->setAttribute("nodedef", "ND_extract_vector3");
                    extractNode->addInputsFromNodeDef();
                    extractNode->getInput("in")->setAttribute("nodename", textureNode->getName());
                    extractNode->getInput("in")->setType("vector3");
                    extractNode->getInput("index")->setAttribute("value", std::to_string(i));
                    if (inputs[i])
                    {
                        inputs[i]->setAttribute("nodename", extractNode->getName());
                        inputs[i]->setType("float");
                    }
                }
            }
            else
            {
                metallicInput->setValue<float>(roughness.metallic_factor);;
                roughnessInput->setValue<float>(roughness.roughness_factor);
            }
        }

        // Normal texture
        InputPtr normalInput = shaderNode->getInput("normal");

        cgltf_texture_view& normalView = material->normal_texture;
        cgltf_texture* normalTexture = normalView.texture;
        if (normalTexture && normalTexture->image)
        {
            std::string imageNodeName = normalTexture->image->name ? normalTexture->image->name :
                "image_normal";
            imageNodeName = _materials->createValidChildName(imageNodeName);
            std::string uri = normalTexture->image->uri ? normalTexture->image->uri : SPACE_STRING;
            NodePtr newTexture = createTexture(_materials, imageNodeName, uri,
                "vector3", EMPTY_STRING);

            std::string normalMapName = _materials->createValidChildName("pbr_normalmap");
            NodePtr normalMap = _materials->addNode("normalmap", normalMapName, "vector3");
            normalMap->setAttribute("nodedef", "ND_normalmap");
            normalMap->addInputsFromNodeDef();
            normalMap->getInput("in")->setAttribute("nodename", newTexture->getName());
            normalMap->getInput("in")->setType("vector3");

            normalInput->setAttribute("nodename", normalMap->getName());
        }

        // Handle sheen
        if (material->has_sheen)
        {
            cgltf_sheen& sheen = material->sheen;

            Color3 colorFactor(sheen.sheen_color_factor[0],
                                    sheen.sheen_color_factor[1],
                                    sheen.sheen_color_factor[2]);
            setColorInput(_materials, shaderNode, "sheen_color",
                colorFactor, &sheen.sheen_color_texture, "image_sheen");

            setFloatInput(_materials, shaderNode, "sheen_roughness",
                sheen.sheen_roughness_factor, &sheen.sheen_roughness_texture,
                "image_sheen_roughness");
        }

        // Handle clearcoat
        if (material->has_clearcoat)
        {
            cgltf_clearcoat& clearcoat = material->clearcoat;

            setFloatInput(_materials, shaderNode, "clearcoat",
                clearcoat.clearcoat_factor, 
                &clearcoat.clearcoat_texture,
                "image_clearcoat");

            setFloatInput(_materials, shaderNode, "clearcoat_roughness",
                clearcoat.clearcoat_roughness_factor, 
                &clearcoat.clearcoat_roughness_texture,
                "image_clearcoat_roughness");           

            // TODO: Handle clearcoat_normal
        }

        // Handle tranmission
        if (material->has_transmission)
        {
            cgltf_transmission& transmission = material->transmission;

            setFloatInput(_materials, shaderNode, "transmission",
                transmission.transmission_factor, 
                &transmission.transmission_texture,
                "image_transmission");
        }

        // Handle specular and specular color
        if (material->has_specular)
        {
            cgltf_specular& specular = material->specular;

            Color3 colorFactor(specular.specular_color_factor[0],
                specular.specular_color_factor[1],
                specular.specular_color_factor[2]);
            setColorInput(_materials, shaderNode, "specular_color",
                colorFactor, 
                &specular.specular_color_texture, 
                "image_specularcolor");

            setFloatInput(_materials, shaderNode, "specular",
                specular.specular_factor, 
                &specular.specular_texture,
                "image_specular");
        }

        // Set ior
        if (material->has_ior)
        {
            cgltf_ior& ior = material->ior;
            InputPtr iorInput = shaderNode->getInput("ior");
            if (iorInput)
            {
                iorInput->setValue<float>(ior.ior);
            }
        }

        // Set emissive inputs
        Color3 colorFactor(material->emissive_factor[0],
            material->emissive_factor[1],
            material->emissive_factor[2]);
        setColorInput(_materials, shaderNode, "emissive",
            colorFactor, &material->emissive_texture, "image_emission");

        if (material->has_emissive_strength)
        {
            cgltf_emissive_strength& emissive_strength = material->emissive_strength;
            InputPtr input = shaderNode->getInput("emissive_strength");
            if (input)
            {
                input->setValue<float>(emissive_strength.emissive_strength);
            }
        }

        // Volume inputs
        if (material->has_volume)
        {
            cgltf_volume& volume = material->volume;

            setFloatInput(_materials, shaderNode, "thickness",
                volume.thickness_factor,
                &volume.thickness_texture,
                "thickness");

            Color3 attenFactor(volume.attenuation_color[0],
                volume.attenuation_color[1],
                volume.attenuation_color[2]);
            setColorInput(_materials, shaderNode, "attenuation_color",
                          attenFactor, nullptr, EMPTY_STRING);

            setFloatInput(_materials, shaderNode, "attenuation_distance",
                volume.attenuation_distance, nullptr, EMPTY_STRING);
        }
    }
}

MATERIALX_NAMESPACE_END
