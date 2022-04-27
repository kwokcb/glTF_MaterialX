
#ifndef MATERIALX_CGLTF_MATERIALLOADER_H
#define MATERIALX_CGLTF_MATERIALLOADER_H

/// @file 
/// GLTF material loader using the Cgltf library

#include <MaterialXRender/Export.h>
#include <MaterialXCore/Document.h>

MATERIALX_NAMESPACE_BEGIN

class CgltfMaterialLoader;

/// Shared pointer to a CgltfMateralLoader
using CgltfMaterialLoaderPtr = std::shared_ptr<class CgltfMaterialLoader>;

/// @class CgltfMateralLoader
/// Wrapper for loader to read materials from GLTF files using the Cgltf library.
class CgltfMaterialLoader
{
  public:
    CgltfMaterialLoader() 
        : _generateFullDefinitions(true)
        , _generateAssignments(false)
    {
        _extensions = { "glb", "GLB", "gltf", "GLTF" };
    }
    virtual ~CgltfMaterialLoader() 
    {
        _materials = nullptr;
    }

    /// Create a new loader
    static CgltfMaterialLoaderPtr create() { return std::make_shared<CgltfMaterialLoader>(); }

    /// Load materials from file path

    /// <summary>
    ///     Convert MaterialX document to glTF and save to file path
    /// </summary>
    /// <param name="filePath">File path</param>
    /// <returns>True on success</returns>
    bool load(const FilePath& filePath);

    /// <summary>
    ///     Convert glTF to MaterialX document and save to file path    
    /// </summary>
    /// <param name="filePath">File path</param>
    /// <returns>True on success</returns>
    bool save(const FilePath& filePath);

    /// <summary>
    ///     Set document containing MaterialX definitions. This includes core library
    ///     definitions
    /// </summary>
    /// <param name="doc">Definition document</param>
    void setDefinitions(DocumentPtr doc)
    {
        _definitions = doc;
    }

    /// <summary>
    ///     Set document to use for MaterialX material generation or extraction.
    /// </summary>
    /// <param name="materials">MaterialX document</param>
    void setMaterials(DocumentPtr materials)
    {
        _materials = materials;
    }

    /// <summary>
    ///     Get MaterialX document containing material information
    /// </summary>
    /// <returns>MaterialX document</returns>
    DocumentPtr getMaterials() const
    {
        return _materials;
    }

    /// <summary>
    ///     Set whether to generate MaterialX assignments if found in the input glTF file.
    ///     By default assignments are not generated.
    /// </summary>
    /// <param name="val">Generate assignments flag</param>
    void setGenerateAssignments(bool val)
    {
        _generateAssignments = val;
    }

    /// <summary>
    ///     Get whether to generate MaterialX material assignments.
    /// </summary>
    /// <returns>True if generating assignments</returns>
    bool getGenerateAssignments() const
    {
        return _generateAssignments;
    }

    /// <summary>
    ///     Set whether to generate all inputs on MaterialX nodes when converting from glTF file.
    ///     By default all inputs are generated.
    /// </summary>
    /// <param name="val">Generate inputsflag</param>
    void setGenerateFullDefinitions(bool val)
    {
        _generateFullDefinitions = val;
    }

    /// <summary>
    ///     Get whether to generate all inputs for MaterialX nodes.
    /// </summary>
    /// <returns>True if generating</returns>
    bool getGenerateFullDefinitions() const
    {
        return _generateFullDefinitions;
    }


  private:
    NodePtr createTexture(DocumentPtr& doc, const std::string & nodeName, const std::string& fileName,
                          const std::string & textureType, const std::string & colorspace);
    void    setColorInput(DocumentPtr materials, NodePtr shaderNode, const std::string& inputName, 
                          const Color3& colorFactor, const void* textureView,
                          const std::string& inputImageNodeName);
    void    setFloatInput(DocumentPtr materials, NodePtr shaderNode, const std::string& inputName, 
                          float floatFactor, const void* textureView,
                          const std::string& inputImageNodeName);

    void loadMaterials(void *);

    StringSet _extensions;
    DocumentPtr _definitions;
    DocumentPtr _materials;

    // Generation options
    bool _generateFullDefinitions;
    bool _generateAssignments;
};

MATERIALX_NAMESPACE_END

#endif
