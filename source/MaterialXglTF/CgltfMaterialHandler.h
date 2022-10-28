
#ifndef MATERIALX_CGLTF_MaterialHandler_H
#define MATERIALX_CGLTF_MaterialHandler_H

/// @file 
/// GLTF material loader using the Cgltf library

#include <MaterialXglTF/Export.h>
#include <MaterialXCore/Document.h>
#include <MaterialXFormat/File.h>

MATERIALX_NAMESPACE_BEGIN

class MaterialHandler;
class CgltfMaterialHandler;

/// Shared pointer to a CgltfMateralLoader
using MaterialHandlerPtr = std::shared_ptr<class MaterialHandler>;
using CgltfMaterialHandlerPtr = std::shared_ptr<class CgltfMaterialHandler>;

/// @class MaterialHandler
/// Wrapper for handler to convert materials to / from MaterialX
class MX_GLTF_API MaterialHandler
{
  public:
    MaterialHandler() 
        : _generateFullDefinitions(false)
        , _generateAssignments(false)
      {}

    virtual ~MaterialHandler() = default;

    /// Load materials from a given file
    virtual bool load(const FilePath& filePath) = 0;

    /// Save materials to a given file
    virtual bool save(const FilePath& filePath) = 0;

    /// Return file extensions supported
    const StringSet& extensionsSupported() const
    {
        return _extensions;
    }

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

  protected:
    StringSet _extensions;
    DocumentPtr _definitions;
    DocumentPtr _materials;

    // Generation options
    bool _generateFullDefinitions;
    bool _generateAssignments;
};

/// @class CgltfMateralLoader
/// Wrapper for loader to read materials from GLTF files using the Cgltf library.
class MX_GLTF_API CgltfMaterialHandler : public MaterialHandler
{
  public:
    CgltfMaterialHandler() 
        : MaterialHandler()
    {
        _extensions = { "glb", "GLB", "gltf", "GLTF" };
    }
    virtual ~CgltfMaterialHandler() 
    {
        _materials = nullptr;
    }


    /// Create a new loader
    static MaterialHandlerPtr create() { return std::make_shared<CgltfMaterialHandler>(); }

    /// Load materials from file path

    /// <summary>
    ///     Convert MaterialX document to glTF and save to file path
    /// </summary>
    /// <param name="filePath">File path</param>
    /// <returns>True on success</returns>
    bool load(const FilePath& filePath) override;

    /// <summary>
    ///     Convert glTF to MaterialX document and save to file path    
    /// </summary>
    /// <param name="filePath">File path</param>
    /// <returns>True on success</returns>
    bool save(const FilePath& filePath) override;

  private:
    NodePtr createColoredTexture(DocumentPtr& doc, const std::string & nodeName, const std::string& fileName,
                                 const Color4& color, const std::string & colorspace);
    NodePtr createTexture(DocumentPtr& doc, const std::string & nodeName, const std::string& fileName,
                          const std::string & textureType, const std::string & colorspace, 
                          const std::string& nodeType = "gltf_image");
    void    setColorInput(DocumentPtr materials, NodePtr shaderNode, const std::string& inputName, 
                          const Color3& color, float alpha, const std::string& alphaInputName, 
                          const void* textureView, const std::string& inputImageNodeName);
    void    setFloatInput(DocumentPtr materials, NodePtr shaderNode, const std::string& inputName, 
                          float floatFactor, const void* textureView,
                          const std::string& inputImageNodeName);
    void    setVector3Input(DocumentPtr materials, NodePtr shaderNode, const std::string& inputName, 
                            const Vector3& vecFactor, const void* textureViewIn,
                            const std::string& inputImageNodeName);
    void    setNormalMapInput(DocumentPtr materials, NodePtr shaderNode, const std::string& inputName, 
                              const void* textureViewIn, const std::string& inputImageNodeName);

    void loadMaterials(void *);
};

MATERIALX_NAMESPACE_END

#endif
