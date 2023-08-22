#ifndef MATERIALX_GLTF_UTIL_H
#define MATERIALX_GLTF_UTIL_H

/// @file 
/// GLTF material handling utilities

#include <MaterialXglTF/Export.h>
#include <MaterialXglTF/GltfMaterialHandler.h>

#include <MaterialXCore/Document.h>
#include <MaterialXFormat/File.h>
#include <MaterialXFormat/Environ.h>
#include <MaterialXFormat/Util.h>
#include <MaterialXFormat/XmlIo.h>

MATERIALX_NAMESPACE_BEGIN

class MX_GLTF_API GltfMaterialUtil
{
  public:
    GltfMaterialUtil()
    {}

    virtual ~GltfMaterialUtil() = default;

    static DocumentPtr glTF2Mtlx(const FilePath& filename, DocumentPtr definitions,
        bool createAssignments, bool fullDefinition, StringVec& logger);

    static bool mtlx2glTF(MaterialHandlerPtr gltfMTLXLoader,
        const FilePath& filename, DocumentPtr materials, StringVec& logger);

    static bool haveSingleDocBake(const FilePath& errorFile);

    static bool bakeDocument(const FilePath& inputFileName, const FilePath& outputFilename,
        unsigned int width, unsigned int height, StringVec& logger);

    static bool renderImage(const FilePath& materialXViewInstall, const FilePath& fileName, const FilePath& meshFile,
        const std::string& outputFileName, StringVec& logger);
};

MATERIALX_NAMESPACE_END

#endif
