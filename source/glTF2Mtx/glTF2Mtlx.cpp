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
#include <MaterialXCore/Document.h>
#include <MaterialXCore/Util.h>
#include <MaterialXFormat/XmlIo.h>
#include <MaterialXFormat/Util.h>

#include <iostream>
#include <MaterialXglTF/GltfMaterialHandler.h>
#include <MaterialXglTF/GltfMaterialUtill.h>

namespace mx = MaterialX;

const std::string options = 
" Options: \n"
"    --mtlx [FILENAME]              Specify the filename to convert to glTF. The --gltf option has priority over this option\n"
"    --gltf [FILENAME]              Specify the filename to convert to MaterialX.\n"
"    --lib [FILEPATH]               Specify the location of the MaterialX definitions libraries. If not specified will try to lose call libraries\n"
"    --assignments                  Specify to generatl material assignments\n"
"    --fullDefinition               Specify to generatel full definitions for each node (all inputs)\n"
"    --help                         Display the complete list of command-line options\n";


mx::FileSearchPath getDefaultSearchPath(const mx::FilePath& materialXLibraryPath)
{
    mx::FileSearchPath searchPath;
    if (!materialXLibraryPath.isEmpty())
    {
        searchPath.append(materialXLibraryPath);
    }

    const mx::FilePath libraryFolder("libraries");
    mx::FilePath modulePath = mx::FilePath::getModulePath();
    if ((modulePath / libraryFolder).exists())
    {
        searchPath.append(modulePath);
    }
    else
    {
        mx::FilePath parentPath = modulePath.getParentPath();
        if ((parentPath / libraryFolder).exists())
        {
            searchPath.append(parentPath);
        }
    }

    return searchPath;
}

int main(int argc, char* const argv[])
{
    std::vector<std::string> tokens;
    for (int i = 1; i < argc; i++)
    {
        tokens.emplace_back(argv[i]);
    }

    mx::FilePath glTFFile;
    mx::FilePath mtlxFile;
    bool createAssignments = false;
    bool fullDefinition = false;
    mx::FilePath materialXLibraryPath;
    for (size_t i = 0; i < tokens.size(); i++)
    {
        const std::string& token = tokens[i];
        const std::string& nextToken = i + 1 < tokens.size() ? tokens[i + 1] : mx::EMPTY_STRING;
        if (token == "--help")
        {
            std::cout << " gltf2Mtlx " << std::endl;
            std::cout << options << std::endl;
            return 0;
        }
        else if (token == "--gltf")
        {
            glTFFile = nextToken;
        }
        else if (token == "--mtlx")
        {
            mtlxFile = nextToken;
        }
        else if (token == "--assignments")
        {
            createAssignments = true;
        }
        else if (token == "--fullDefinition")
        {
            fullDefinition = true;
        }        
        else if (token == "--lib")
        {
            materialXLibraryPath = nextToken;
        }
    }

    // Load in MaterialX library definitions
    //
    mx::FilePath modulePath = mx::FilePath::getModulePath();
    mx::FilePath currentPath = mx::FilePath::getCurrentPath();
    mx::FilePathVec libraryFolders; 
    libraryFolders.push_back(mx::FilePath("libraries"));
    mx::FileSearchPath searchPath = getDefaultSearchPath(materialXLibraryPath);
    mx::DocumentPtr stdLib = mx::createDocument();
    mx::StringSet xincludeFiles = mx::loadLibraries(libraryFolders, searchPath, stdLib);
    if (xincludeFiles.empty())
    {
        std::cerr << "Could not load library definitions" << std::endl;
        return -1;
    }

    bool toMaterialX = !glTFFile.isEmpty();

    // Convert from glTF to MaterialX
    if (!toMaterialX)
    {
        if (mtlxFile.isEmpty())
        {
            std::cerr << "Insufficient arguments specified." << std::endl;
            std::cerr << "\n gltf2Mtlx " << std::endl;
            std::cerr << options << std::endl;
            return -1;
        }

        mx::FilePath testPath;
        if (!mtlxFile.isAbsolute())
        {
            testPath = currentPath / mtlxFile;
            if (testPath.exists())
            {
                mtlxFile = testPath;
            }
            else 
            {
                testPath = currentPath.getParentPath() / mtlxFile;
                if (testPath.exists())
                {
                    mtlxFile = testPath;
                }
            }
        }
        if (!mtlxFile.exists())
        {
            std::cerr << "Input MaterialX file does not exist: " << mtlxFile.asString() << std::endl;
            return -1;
        }

        mx::DocumentPtr materials = mx::createDocument();
        materials->importLibrary(stdLib);
        mx::readFromXmlFile(materials, mtlxFile);
        mx::FilePath outputFile = mtlxFile;
        outputFile.addExtension("_converted.gltf");

        mx::MaterialHandlerPtr gltfMTLXLoader = mx::GltfMaterialHandler::create();
        mx::StringVec log;
        if (mx::GltfMaterialUtil::mtlx2glTF(gltfMTLXLoader, outputFile, materials, log))
        {
            std::cout << "Wrote glTF file to: " << outputFile.asString() << std::endl;
        }
        else
        {
            std::cout << "Failed to write glTF file to: " << outputFile.asString() << std::endl;
            for (const std::string& error : log)
            {
                std::cout << error << std::endl;
            }
        }
    }
    else
    {
        if (glTFFile.isEmpty())
        {
            std::cerr << "Input GLTF file not specified." << std::endl;
            return -1;
        }
        if (!glTFFile.isAbsolute())
        {
            mx::FilePath testPath = currentPath / glTFFile;
            if (testPath.exists())
            {
                glTFFile = testPath;
            }
            else 
            {
                testPath = currentPath.getParentPath() / glTFFile;
                if (testPath.exists())
                {
                    glTFFile = testPath;
                }
            }
        }
        if (!glTFFile.exists())
        {
            std::cerr << "Input GLTF file does not exist: " << glTFFile.asString() << std::endl;
            return -1;
        }
        else
        {
            std::cout << "current path: " << currentPath.asString() << std::endl;
            std::cout << "Convert GLTF: " << glTFFile.asString() << std::endl;
        }
        mx::DocumentPtr materials = nullptr;
        mx::StringVec log;
        try {
             materials = mx::GltfMaterialUtil::glTF2Mtlx(glTFFile, stdLib, createAssignments, fullDefinition, log);
        }
        catch (std::exception&)
        {
            std::cerr << "Failed to translate" << std::endl;
            for (const std::string& error : log)
            {
                std::cout << error << std::endl;
            }
        }
        if (materials)
        {
            mx::XmlWriteOptions writeOptions;
            writeOptions.elementPredicate = [](mx::ConstElementPtr elem)
            {
                if (elem->hasSourceUri())
                {
                    return false;
                }
                return true;
            };
            const std::string outputFileName = glTFFile.asString() + ".mtlx";
            std::cout << "Wrote MaterialX File: " << outputFileName << std::endl;
            mx::writeToXmlFile(materials, glTFFile.asString() + ".mtlx", &writeOptions);
        }
        else
        {
            std::cout << "Failed to write MaterialX File" << std::endl;
        }
    }
    return 0;
}
