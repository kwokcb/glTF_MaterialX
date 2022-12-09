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
        bool createAssignments, bool fullDefinition);

    static bool mtlx2glTF(MaterialHandlerPtr gltfMTLXLoader,
        const FilePath& filename, DocumentPtr materials);

    static bool haveSingleDocBake(const FilePath& errorFile);

    static bool bakeDocument(const FilePath& inputFileName, const FilePath& outputFilename,
        unsigned int width, unsigned int height, std::ostream& logFile);

    static bool renderCheck(FilePath& fileName, FilePath fullPath, const std::string& outputFileName,
        std::ostream& logFile);
};

MATERIALX_NAMESPACE_END

#endif