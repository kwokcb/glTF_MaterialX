
#include <glTFMtlxTest/Catch/catch.hpp>

#include <MaterialXglTF/CgltfMaterialHandler.h>
#include <MaterialXFormat/Environ.h>
#include <MaterialXFormat/Util.h>
#include <MaterialXFormat/XmlIo.h>

#include <fstream>
#include <iostream>
#include <limits>
#include <unordered_set>

namespace mx = MaterialX;

mx::DocumentPtr glTF2Mtlx(const mx::FilePath& filename, mx::DocumentPtr definitions, 
                          bool createAssignments, bool fullDefinition)
{
    mx::MaterialHandlerPtr gltfMTLXLoader = mx::CgltfMaterialHandler::create();
    gltfMTLXLoader->setDefinitions(definitions);
    gltfMTLXLoader->setGenerateAssignments(createAssignments);
    gltfMTLXLoader->setGenerateFullDefinitions(fullDefinition);
    bool loadedMaterial = gltfMTLXLoader->load(filename);
    mx::DocumentPtr materials = loadedMaterial ? gltfMTLXLoader->getMaterials() : nullptr;
    return materials;
}

// MaterialX to cgTF conversion
bool mtlx2glTF(const mx::FilePath& filename, mx::DocumentPtr materials)
{
    mx::MaterialHandlerPtr gltfMTLXLoader = mx::CgltfMaterialHandler::create();
    gltfMTLXLoader->setMaterials(materials);
    return gltfMTLXLoader->save(filename);
}

TEST_CASE("glTF: Valid glTF -> MTLX", "[gltf]")
{
    mx::DocumentPtr libraries = mx::createDocument();
    mx::FileSearchPath searchPath;
    searchPath.append(mx::FilePath::getCurrentPath());
    mx::StringSet xincludeFiles = loadLibraries({ "libraries" }, searchPath, libraries);

    mx::XmlWriteOptions writeOptions;
    writeOptions.elementPredicate = [](mx::ConstElementPtr elem)
    {
        if (elem->hasSourceUri())
        {
            return false;
        }
        return true;
    };

    // Scan for glTF sample mode files in resources directory
    const mx::FilePath currentPath = mx::FilePath::getCurrentPath();
    mx::FilePath rootPath = currentPath / "resources/";

    // Check if an environment variable was set as the root
    if (!rootPath.exists())
    {
        rootPath = mx::getEnviron("GLTF_SAMPLE_MODELS_ROOT");
    }
    if (!rootPath.exists())
    {
        std::cout << "glTF sample models not found: " << rootPath.asString() << ". Skipping test" << std::endl;
        return;
    }

    bool createAssignments = true;
    bool fullDefinition = false;
    const std::string GLTF_EXTENSION("gltf");
    for (const mx::FilePath& dir : rootPath.getSubDirectories())
    {
        if (std::string::npos != dir.asString().find("glTF-Binary") || 
            std::string::npos != dir.asString().find("glTF-Draco") || 
            std::string::npos != dir.asString().find("glTF-Embedded"))
        {
            continue;
        }
        for (const mx::FilePath& gltfFile : dir.getFilesInDirectory(GLTF_EXTENSION))
        {
            mx::FilePath fullPath = dir / gltfFile;
            mx::DocumentPtr materials = glTF2Mtlx(fullPath, libraries, createAssignments, fullDefinition);
            if (materials)
            {
                std::vector<mx::NodePtr> nodes = materials->getMaterialNodes();
                if (nodes.size())
                {
                    std::string message;
                    if (!materials->validate(&message))
                    {
                        std::cerr << "*** Validation warnings document created from: " << fullPath.asString() << " ***" << std::endl;
                        std::cerr << message;
                    }
                    mx::FilePath outputPath = fullPath;
                    outputPath.removeExtension();
                    const std::string outputFileName = outputPath.asString() + "_converted.mtlx";
                    std::cout << "Wrote " << std::to_string(nodes.size()) << " materials to file : " << outputFileName << std::endl;
                    mx::writeToXmlFile(materials, outputFileName, &writeOptions);

                    const std::string outputFileName2 = outputPath.asString() + "_converted.gltf";
                    bool converted = mtlx2glTF(outputFileName2, materials);
                    CHECK(converted);
                    if (converted)
                    {
                        std::cout << "Wrote MTLX materials to glTF file : " << outputFileName2 << std::endl;
                    }
                }
            }
        }
    }
}