
#include <glTFMtlxTest/Catch/catch.hpp>

#include <MaterialXglTF/GltfMaterialHandler.h>
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
    mx::MaterialHandlerPtr gltfMTLXLoader = mx::GltfMaterialHandler::create();
    gltfMTLXLoader->setDefinitions(definitions);
    gltfMTLXLoader->setGenerateAssignments(createAssignments);
    gltfMTLXLoader->setGenerateFullDefinitions(fullDefinition);
    bool loadedMaterial = gltfMTLXLoader->load(filename);
    mx::DocumentPtr materials = loadedMaterial ? gltfMTLXLoader->getMaterials() : nullptr;
    return materials;
}

// MaterialX to cgTF conversion
bool mtlx2glTF(mx::MaterialHandlerPtr gltfMTLXLoader, const mx::FilePath& filename, mx::DocumentPtr materials)
{    
    gltfMTLXLoader->setMaterials(materials);
    return gltfMTLXLoader->save(filename);
}

bool translateShader(const mx::FilePath& inputFileName, const mx::FilePath& outputFilename, 
                     unsigned int width, unsigned int height)
{
    // Run test renders on output
    mx::FilePath shaderTranslator(MTLX_TRANSLATE_SHADER);
    if (!shaderTranslator.isEmpty())
    {
//python d:\Work\materialx\bernard_MaterialX\build\installed\python\Scripts\translateshader.py 
//build\bin\resources\glTF_export\Materials\Examples\StandardSurface\standard_surface_brass_tiled.mtlx final.mtlx gltf_pbr

        const std::string errorFile = inputFileName.asString() + "_errors.txt";
        const std::string redirectString(" 2>&1");
        const std::string target = "gltf_pbr";

        std::string command = "python " + shaderTranslator.asString()
            + " --width " + std::to_string(width)
            + " --height " + std::to_string(height)
            + " " + inputFileName.asString()
            + " " + outputFilename.asString()
            + " " + target
            + " > " + errorFile + redirectString;

        std::cout << "- Translated MTLX: " + outputFilename.asString() << std::endl;
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
        return true;
        //CHECK(!renderError);
    }
    return false;
}

// MaterialX to glTF to MaterialX tests
TEST_CASE("Validate export", "[gltf]")
{
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
        std::cout << "MaterialX export directory used: " << rootPath.asString() << std::endl;
    }
    else
    {
        rootPath = "resources/glTF_export/Materials/Examples";
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
                std::cout << "------------ Skip file: " << fullPath.asString() << std::endl;
                continue;
            }

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
            gltfMTLXLoader->translatgeShaders(doc);
            std::cout << "- Wrote distilled file : " << distilledFileName << std::endl;
            mx::writeToXmlFile(doc, distilledFileName, &writeOptions);
            testedFiles.insert(distilledFileName);

            // Bake to a new document
            const mx::FilePath bakedFileName = fileName.asString() + "_baked.mtlx";
            translateShader(distilledFileName, bakedFileName, 512, 512);
            testedFiles.insert(bakedFileName);
            if (bakedFileName.exists())
            {
                mx::DocumentPtr bakedDoc = mx::createDocument();
                bakedDoc->importLibrary(libraries);
                readFromXmlFile(bakedDoc, bakedFileName);
                CHECK(doc->validate());

                const std::string outputFileName = fileName.asString() + "_fromtlx.gltf";
                bool convertedToGLTF = mtlx2glTF(gltfMTLXLoader, outputFileName, bakedDoc);
                if (convertedToGLTF)
                {
                    std::cout << "Converted to gltf: " << outputFileName << std::endl;
                }
            }
        }
    }
}

// glTF to MaterialX to glTF tests
TEST_CASE("Validate import", "[gltf]")
{
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
        std::cout << "glTF sample models directory used: " << rootPath.asString() << std::endl;
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
        std::cout << "Test glTF directory not found: " << rootPath.asString() << ". Skipping test" << std::endl;
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

                    mx::MaterialHandlerPtr gltfMTLXLoader = mx::GltfMaterialHandler::create();
                    const std::string outputFileName2 = fileName.asString() + "_fromtlx.gltf";
                    bool convertedToGLTF = mtlx2glTF(gltfMTLXLoader, outputFileName2, materials);
                    CHECK(convertedToGLTF);
                    if (convertedToGLTF)
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