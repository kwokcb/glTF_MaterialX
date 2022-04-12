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

mx::FileSearchPath getDefaultSearchPath()
{
    mx::FilePath modulePath = mx::FilePath::getModulePath();
    mx::FilePath installRootPath = modulePath.getParentPath();
    mx::FilePath devRootPath = installRootPath.getParentPath().getParentPath();

    mx::FileSearchPath searchPath;
    if ((devRootPath / "libraries").exists())
    {
        searchPath.append(devRootPath);
    }
    else
    {
        searchPath.append(installRootPath);
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
    for (size_t i = 0; i < tokens.size(); i++)
    {
        const std::string& token = tokens[i];
        const std::string& nextToken = i + 1 < tokens.size() ? tokens[i + 1] : mx::EMPTY_STRING;
        if (token == "--gltf")
        {
            glTFFile = nextToken;
        }
    }

    if (!glTFFile.exists())
    {
        std::cerr << "Input file does not exist: " << glTFFile.asString();
        return -1;
    }

    mx::FilePathVec _libraryFolders; 
    _libraryFolders.push_back(mx::FilePath("libraries"));
    mx::FileSearchPath _searchPath = getDefaultSearchPath();
    mx::DocumentPtr _stdLib = mx::createDocument();
    mx::StringSet _xincludeFiles = mx::loadLibraries(_libraryFolders, _searchPath, _stdLib);

    bool toMaterialX = true;
    if (toMaterialX)
    {
        mx::DocumentPtr materials = glTF2Mtlx(glTFFile, _stdLib);
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
            mx::writeToXmlFile(materials, glTFFile.asString() + ".mtlx", &writeOptions);
        }
    }
    return 0;
}
