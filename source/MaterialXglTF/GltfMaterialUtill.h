/* 

Copyright 2022 - 2023 Bernard Kwok

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http ://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/
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
