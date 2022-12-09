
#include <glTFMtlxTest/Catch/catch.hpp>

#include <MaterialXglTF/GltfMaterialHandler.h>
#include <MaterialXglTF/GltfMaterialUtill.h>
#include <MaterialXFormat/Environ.h>
#include <MaterialXFormat/Util.h>
#include <MaterialXFormat/XmlIo.h>

#include <fstream>
#include <iostream>
#include <limits>
#include <unordered_set>

namespace mx = MaterialX;

// MaterialX to glTF to MaterialX tests
TEST_CASE("Validate export", "[gltf]")
{
    std::ofstream logFile;
    logFile.open("gltf_export_log.txt");

    const std::string GLTF_EXTENSION("gltf");
    const std::string MTLX_EXTENSION("mtlx");

    mx::DocumentPtr libraries = mx::createDocument();
    mx::FileSearchPath searchPath;
    searchPath.append(mx::FilePath::getCurrentPath());
    searchPath.append(mx::FilePath::getModulePath());
    searchPath.append(mx::FilePath::getModulePath().getParentPath());
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

    bool useExternalPath = false;
    mx::FilePath rootPath = mx::getEnviron("GLTF_SAMPLE_MODELS_ROOT");
    if (!rootPath.isEmpty())
    {
        useExternalPath = true;
        logFile << "MaterialX export directory used: " << rootPath.asString() << std::endl;
    }
    else
    {
        rootPath = "resources/glTF_export/Materials/Examples/StandardSurface";
        rootPath = searchPath.find(rootPath);
    }

    mx::StringSet testedFiles;
    for (const mx::FilePath& dir : rootPath.getSubDirectories())
    {
        for (const mx::FilePath& gltfFile : dir.getFilesInDirectory(MTLX_EXTENSION))
        {
            mx::FilePath fullPath = dir / gltfFile;

            if (testedFiles.count(fullPath) || 
                std::string::npos != fullPath.asString().find("distilled") ||
                std::string::npos != fullPath.asString().find("baked"))
            {
                //logFile << "------------ Skip file: " << fullPath.asString() << std::endl;
                continue;
            }

            logFile << "* Convert from MaterialX to gltF: " << fullPath.asString() << std::endl;
            std::cerr << "* Convert from MaterialX to gltF: " << fullPath.asString() << std::endl;

            mx::DocumentPtr doc = mx::createDocument();
            doc->importLibrary(libraries);
            readFromXmlFile(doc, fullPath);
            CHECK(doc->validate());

            mx::FilePath fileName = fullPath.getBaseName();
            fileName.removeExtension();
            fileName = dir / fileName;

            mx::MaterialHandlerPtr gltfMTLXLoader = mx::GltfMaterialHandler::create();

            // Perform shader translation in place
            const std::string distilledFileName = fileName.asString() + "_distilled.mtlx";
            gltfMTLXLoader->translateShaders(doc);
            logFile << "  * Wrote distilled file : " << distilledFileName << std::endl;
            mx::writeToXmlFile(doc, distilledFileName, &writeOptions);
            testedFiles.insert(distilledFileName);

            // Bake to a new document
            const mx::FilePath bakedFileName = fileName.asString() + "_baked.mtlx";
            mx::GltfMaterialUtil::bakeDocument(distilledFileName, bakedFileName, 512, 512, logFile);
            testedFiles.insert(bakedFileName);
            if (bakedFileName.exists())
            {
                logFile << "  * Created bake file: " << bakedFileName.asString() << std::endl;
                mx::DocumentPtr bakedDoc = mx::createDocument();
                bakedDoc->importLibrary(libraries);
                readFromXmlFile(bakedDoc, bakedFileName);
                CHECK(doc->validate());

                const std::string outputFileName = fileName.asString() + "_fromtlx.gltf";
                bool convertedToGLTF = mx::GltfMaterialUtil::mtlx2glTF(gltfMTLXLoader, outputFileName, bakedDoc);
                if (convertedToGLTF)
                {
                    logFile << "  * Converted to gltf: " << outputFileName << std::endl;

                    mx::DocumentPtr materials = mx::GltfMaterialUtil::glTF2Mtlx(outputFileName, libraries, true, false);
                    if (materials)
                    {
                        logFile << "  * RE-converted back to MTLX: " << (outputFileName + "_reimmport.mtlx") << std::endl;
                        std::cerr << "  * RE-converted back to MTLX: " << (outputFileName + "_reimmport.mtlx") << std::endl;
                        mx::writeToXmlFile(materials, outputFileName + "_reimmport.mtlx");
                    }
                }
                else
                {
                    logFile << "  * Failed to convert to gltf: " << outputFileName << std::endl;
                }
            }
            else
            {
                logFile << "  * Failed to bake: " << bakedFileName.asString() << std::endl;
            }
        }
    }

    logFile.close();
}

// glTF to MaterialX to glTF tests
TEST_CASE("Validate import", "[gltf]")
{
    std::ofstream logFile;
    logFile.open("gltf_import_log.txt");

    const std::string GLTF_EXTENSION("gltf");
    const std::string MTLX_EXTENSION("mtlx");

    mx::DocumentPtr libraries = mx::createDocument();
    mx::FileSearchPath searchPath;
    searchPath.append(mx::FilePath::getCurrentPath());
    searchPath.append(mx::FilePath::getModulePath());
    searchPath.append(mx::FilePath::getModulePath().getParentPath());
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
    bool useSampleModels = false;
    mx::FilePath rootPath = mx::getEnviron("GLTF_SAMPLE_MODELS_ROOT");
    if (!rootPath.isEmpty())
    {
        useSampleModels = true;
        logFile << "glTF sample models directory used: " << rootPath.asString() << std::endl;
    }
    else
    {
        rootPath = "resources/glTF_import";
        rootPath = searchPath.find(rootPath);
    }

    // Check if an environment variable was set as the root
    if (!rootPath.exists())
    {
    }
    if (!rootPath.exists())
    {
        logFile << "Test glTF directory not found: " << rootPath.asString() << ". Skipping test" << std::endl;
        logFile.close();
        return;
    }

    bool createAssignments = true;
    bool fullDefinition = false;
    mx::StringSet testedFiles;
    for (const mx::FilePath& dir : rootPath.getSubDirectories())
    {
        // If sample models root is set, then skip the following directories
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

            std::cerr << "* Convert from glTF to MTLX: " << fullPath.asString() << std::endl;
            logFile << "* Convert from glTF to MTLX: " << fullPath.asString() << std::endl;

            mx::DocumentPtr materials = mx::GltfMaterialUtil::glTF2Mtlx(fullPath, libraries, createAssignments, fullDefinition);
            if (materials)
            {
                std::vector<mx::NodePtr> nodes = materials->getMaterialNodes();
                if (nodes.size())
                {
                    std::string message;
                    if (!materials->validate(&message))
                    {
                        logFile << "- Validation warnings document created from: " << fullPath.asString() << " ***" << std::endl;
                        logFile << message;
                    }
                    mx::FilePath fileName = fullPath.getBaseName();
                    fileName.removeExtension();
                    fileName = dir / fileName;

                    const std::string outputFileName = fileName.asString() + "_fromgltf.mtlx";
                    logFile << "  * Wrote " << std::to_string(nodes.size()) << " materials to file : " << outputFileName << std::endl;
                    mx::writeToXmlFile(materials, outputFileName, &writeOptions);

                    mx::MaterialHandlerPtr gltfMTLXLoader = mx::GltfMaterialHandler::create();
                    const std::string outputFileName2 = fileName.asString() + "_fromtlx.gltf";
                    bool convertedToGLTF = mx::GltfMaterialUtil::mtlx2glTF(gltfMTLXLoader, outputFileName2, materials);
                    CHECK(convertedToGLTF);
                    if (convertedToGLTF)
                    {
                        testedFiles.insert(outputFileName2);
                        logFile << "  * Wrote MTLX materials to glTF file : " << outputFileName2 << std::endl;
                    }

                    bool runRender = true;
                    if (runRender)
                    {
                        bool renderError = mx::GltfMaterialUtil::renderCheck(fileName, fullPath, outputFileName, logFile);
                        CHECK(!renderError);
                    }
                }
            }
        }
    }

    logFile.close();
}