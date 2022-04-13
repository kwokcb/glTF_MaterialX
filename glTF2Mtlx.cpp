#include <MaterialXCore/Document.h>
#include <MaterialXCore/Util.h>
#include <MaterialXFormat/XmlIo.h>
#include <MaterialXFormat/Util.h>

#include <iostream>
#include "CgltfMaterialLoader.h"

namespace mx = MaterialX;

// glTF to MaterialX conversion
mx::DocumentPtr glTF2Mtlx(const mx::FilePath& filename, mx::DocumentPtr definitions)
{
    mx::CgltfMaterialLoaderPtr gltfMTLXLoader = mx::CgltfMaterialLoader::create();
    gltfMTLXLoader->setDefinitions(definitions);
    bool loadedMaterial = gltfMTLXLoader->load(filename);
    mx::DocumentPtr materials = loadedMaterial ? gltfMTLXLoader->getMaterials() : nullptr;
    return materials;
}

// MaterialX to cgTF conversion -- TODO
void mtlx2glTF()
{

}

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
    mx::FilePath materialXLibraryPath;
    for (size_t i = 0; i < tokens.size(); i++)
    {
        const std::string& token = tokens[i];
        const std::string& nextToken = i + 1 < tokens.size() ? tokens[i + 1] : mx::EMPTY_STRING;
        if (token == "--gltf")
        {
            glTFFile = nextToken;
        }
        else if (token == "--lib")
        {
            materialXLibraryPath = nextToken;
        }
    }

    mx::FilePath modulePath = mx::FilePath::getModulePath();
    if (!glTFFile.isAbsolute())
    {
        glTFFile = modulePath / glTFFile;
    }
    if (!glTFFile.exists())
    {
        std::cerr << "Input file does not exist: " << glTFFile.asString();
        return -1;
    }

    mx::FilePathVec libraryFolders; 
    libraryFolders.push_back(mx::FilePath("libraries"));
    mx::FileSearchPath searchPath = getDefaultSearchPath(materialXLibraryPath);
    mx::DocumentPtr stdLib = mx::createDocument();
    mx::StringSet xincludeFiles = mx::loadLibraries(libraryFolders, searchPath, stdLib);
    if (xincludeFiles.empty())
    {
        std::cerr << "Could not load library definitions using paths";
        return -1;
    }

    bool toMaterialX = true;
    if (toMaterialX)
    {
        mx::DocumentPtr materials = glTF2Mtlx(glTFFile, stdLib);
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
            std::cout << "Wrote MaterialX File: " << outputFileName;
            mx::writeToXmlFile(materials, glTFFile.asString() + ".mtlx", &writeOptions);
        }
    }
    return 0;
}
