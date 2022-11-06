
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
    mx::FilePath rootPath = "resources/";

    // Check if an environment variable was set as the root
    bool useSampleModels = false;
    if (!rootPath.exists())
    {
        rootPath = mx::getEnviron("GLTF_SAMPLE_MODELS_ROOT");
        std::cout << "glTF sample models directory used: " << rootPath.asString() << std::endl;
    }
    if (!rootPath.exists())
    {
        std::cout << "Test glTF directory not found: " << rootPath.asString() << ". Skipping test" << std::endl;
        return;
    }

    bool createAssignments = true;
    bool fullDefinition = false;
    const std::string GLTF_EXTENSION("gltf");
    mx::StringSet testedFiles;
    for (const mx::FilePath& dir : rootPath.getSubDirectories())
    {
        // If sample models directory is set, then skip the following directories
        if (useSampleModels)
        {
            if (std::string::npos != dir.asString().find("glTF-Binary") ||
                std::string::npos != dir.asString().find("glTF-Draco") ||
                std::string::npos != dir.asString().find("glTF-Embedded"))
            {
                continue;
            }
        }

        bool createdOutputDirectories = false;
        const std::string OUTPUT_MATERIAL_FOLDER = "converted_materials";

        for (const mx::FilePath& gltfFile : dir.getFilesInDirectory(GLTF_EXTENSION))
        {
            mx::FilePath fullPath = dir / gltfFile;

            if (testedFiles.count(fullPath))
            {
                continue;
            }

            mx::DocumentPtr materials = glTF2Mtlx(fullPath, libraries, createAssignments, fullDefinition);
            if (materials)
            {
                std::vector<mx::NodePtr> nodes = materials->getMaterialNodes();
                if (nodes.size())
                {
                    std::string message;
                    if (!materials->validate(&message))
                    {
                        std::cerr << "- Validation warnings document created from: " << fullPath.asString() << " ***" << std::endl;
                        std::cerr << message;
                    }
                    mx::FilePath fileName = fullPath.getBaseName();
                    fileName.removeExtension();
                    fileName = dir / fileName;

                    const std::string outputFileName = fileName.asString() + "_fromgltf.mtlx";
                    std::cout << "- Wrote " << std::to_string(nodes.size()) << " materials to file : " << outputFileName << std::endl;
                    mx::writeToXmlFile(materials, outputFileName, &writeOptions);

                    const std::string outputFileName2 = fileName.asString() + "_fromtlx.gltf";
                    bool converted = mtlx2glTF(outputFileName2, materials);
                    CHECK(converted);
                    if (converted)
                    {
                        testedFiles.insert(outputFileName2);
                        std::cout << "- Wrote MTLX materials to glTF file : " << outputFileName2 << std::endl;
                    }

                    // Run test renders on output
                    mx::FilePath materialXInstallRoot(MTLXVIEW_TEST_RENDER);
                    if (!materialXInstallRoot.isEmpty())
                    {

                        const std::string imageFileName = fileName.asString() + ".png";
                        const std::string errorFile = fileName.asString() + "_errors.txt";
                        const std::string redirectString(" 2>&1");

                        std::string command = materialXInstallRoot.asString()
                            + " --mesh " + fullPath.asString()
                            + " --material " + outputFileName
                            + " --screenWidth 512 --screenHeight 512 "
                            + " --captureFilename " + imageFileName
                            + " --envSampleCount 4"
                            + " > " + errorFile + redirectString;

                        std::cout << "- Render image: " + imageFileName << std::endl;
                        int returnValue = std::system(command.c_str());

                        std::ifstream errorStream(errorFile);
                        std::string result;
                        result.assign(std::istreambuf_iterator<char>(errorStream),
                            std::istreambuf_iterator<char>());

                        bool renderError = returnValue != 0 && !result.empty();
                        if (renderError)
                        {
                            mx::StringVec errors;
                            std::cout << "- Errors: " << std::endl;
                            std::cout << "  - Command string : " + command << std::endl;
                            std::cout << "  - Command return code: " + std::to_string(returnValue) << std::endl;
                            std::cout << "  - Log: " << result << std::endl;
                        }
                        CHECK(!renderError);
                    }
                }
            }
        }
    }
}