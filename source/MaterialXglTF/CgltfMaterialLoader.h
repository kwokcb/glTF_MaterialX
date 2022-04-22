
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
class /*MX_RENDER_API*/ CgltfMaterialLoader
{
  public:
    CgltfMaterialLoader()
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
    bool load(const FilePath& filePath);

    /// Save materials to file path
    bool save(const FilePath& filePath);

    void setDefinitions(DocumentPtr doc)
    {
        _definitions = doc;
    }

    void setMaterials(DocumentPtr materials)
    {
        _materials = materials;
    }

    DocumentPtr getMaterials() const
    {
        return _materials;
    }

  private:
    void loadMaterials(void *);

    StringSet _extensions;
    DocumentPtr _definitions;
    DocumentPtr _materials;
};

MATERIALX_NAMESPACE_END

#endif
