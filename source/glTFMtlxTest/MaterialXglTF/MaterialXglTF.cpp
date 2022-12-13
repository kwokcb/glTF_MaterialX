
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

mx::FilePath getRendererPath()
{
#if defined(GLTF_MATERIALX_TEST_RENDER)
    mx::FilePath mtlxViewExe =  mx::getEnviron("MTLXVIEW_TEST_RENDER");
    if (mtlxViewExe.isEmpty())
    {
#if defined(_MSC_VER)
        mtlxViewExe = mx::FilePath::getModulePath() / mx::FilePath("MaterialXView.exe");
#else        
        mtlxViewExe = mx::FilePath::getModulePath() / mx::FilePath("MaterialXView");
#endif        
    }
    if (!mtlxViewExe.exists())
    {
        mtlxViewExe = mx::EMPTY_STRING;
    }
#else
    mx::FilePath mtlxViewExe = mx::EMPTY_STRING;
#endif

    return mtlxViewExe;
}

// MaterialX to glTF to MaterialX tests
TEST_CASE("Validate MaterialX to glTF export", "[gltf_export]")
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

    // Check if we can run render tests
    mx::FilePath mtlxViewExe = getRendererPath();
    bool runRenderTest = !mtlxViewExe.isEmpty();
    if (runRenderTest)
    {
        logFile << "** MaterialXView executable is: " << mtlxViewExe.asString() << std::endl;
    }

    mx::StringSet testedFiles;
    for (const mx::FilePath& dir : rootPath.getSubDirectories())
    {
        for (const mx::FilePath& gltfFile : dir.getFilesInDirectory(MTLX_EXTENSION))
        {
            mx::FilePath fullPath = dir / gltfFile;

            if (testedFiles.count(fullPath) || 
                std::string::npos != fullPath.asString().find("reimported") ||
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
            gltfMTLXLoader->translateShaders(doc, logFile);
            CHECK(doc->validate());

            // Try to bake
            logFile << "  * Wrote distilled file : " << distilledFileName << std::endl;
            mx::writeToXmlFile(doc, distilledFileName, &writeOptions);
            testedFiles.insert(distilledFileName);

            const mx::FilePath bakedFileName = fileName.asString() + "_baked.mtlx";
            mx::GltfMaterialUtil::bakeDocument(distilledFileName, bakedFileName, 512, 512, logFile);
            testedFiles.insert(bakedFileName);
            if (bakedFileName.exists())
            {
                logFile << "  * Baked to file: " << bakedFileName.asString() << std::endl;
                doc= mx::createDocument();
                doc->importLibrary(libraries);
                readFromXmlFile(doc, bakedFileName);
            }
            else
            {
                logFile << "  * Did not perform bake" << std::endl;
            }

            // Convert MTLX document to glTF
            const std::string outputFileName = fileName.asString() + "_fromtlx.gltf";
            bool convertedToGLTF = mx::GltfMaterialUtil::mtlx2glTF(gltfMTLXLoader, outputFileName, doc, logFile);
            if (convertedToGLTF)
            {
                logFile << "  * Converted to gltf: " << outputFileName << std::endl;
                testedFiles.insert(outputFileName);

                mx::DocumentPtr materials = mx::GltfMaterialUtil::glTF2Mtlx(outputFileName, libraries, true, false, logFile);
                if (materials)
                {
                    mx::FilePath reimportedFile = outputFileName + "_reimport.mtlx";
                    testedFiles.insert(reimportedFile);
                    logFile << "  * RE-converted back to MTLX: " << reimportedFile.asString() << std::endl;
                    std::cerr << "  * RE-converted back to MTLX: " << reimportedFile.asString() << std::endl;
                    mx::writeToXmlFile(materials, reimportedFile, &writeOptions);
                    if (runRenderTest)
                    {
                        std::cerr << "  * Render re-converted MTLX file" << std::endl;
                        logFile << "  * Render re-converted MTLX file" << std::endl;
                        const mx::FilePath captureName = reimportedFile.asString() + ".png";
                        const mx::FilePath meshPath;
                        const mx::FilePath materialPath = reimportedFile;
                        bool renderError = mx::GltfMaterialUtil::renderImage(mtlxViewExe, captureName, meshPath, materialPath, logFile);
                        CHECK(!renderError);
                    }
                }
            }
            else
            {
                logFile << "  * Failed to convert to gltf: " << outputFileName << std::endl;
            }
        }
    }

    logFile.close();
}

// glTF to MaterialX to glTF tests
TEST_CASE("Validate gltf to MaterialX import", "[gltf_import]")
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
        logFile << "Test glTF directory not found: " << rootPath.asString() << ". Skipping test" << std::endl;
        logFile.close();
        return;
    }

    // Check if we can run render tests
    mx::FilePath mtlxViewExe = getRendererPath();
    bool runRenderTest = !mtlxViewExe.isEmpty();
    if (runRenderTest)
    {
        logFile << "** MaterialXView executable is: " << mtlxViewExe.asString() << std::endl;
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

            mx::DocumentPtr materials = mx::GltfMaterialUtil::glTF2Mtlx(fullPath, libraries, createAssignments, fullDefinition, logFile);
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
                    bool convertedToGLTF = mx::GltfMaterialUtil::mtlx2glTF(gltfMTLXLoader, outputFileName2, materials, logFile);
                    CHECK(convertedToGLTF);
                    if (convertedToGLTF)
                    {
                        testedFiles.insert(outputFileName2);
                        logFile << "  * Wrote MTLX materials to glTF file : " << outputFileName2 << std::endl;
                    }

                    if (runRenderTest)
                    {
                        const mx::FilePath captureName = fileName.asString() + ".png";
                        const mx::FilePath meshPath = fullPath;
                        const mx::FilePath materialPath = outputFileName;
                        bool renderError = mx::GltfMaterialUtil::renderImage(mtlxViewExe, captureName, meshPath, materialPath, logFile);
                        CHECK(!renderError);
                    }
                }
            }
        }
    }

    logFile.close();
}