
#include <MaterialXglTF/GltfMaterialUtill.h>

#include <fstream>
#include <iostream>
#include <limits>
#include <unordered_set>

MATERIALX_NAMESPACE_BEGIN

DocumentPtr GltfMaterialUtil::glTF2Mtlx(
                const FilePath& filename, DocumentPtr definitions, 
                bool createAssignments, bool fullDefinition)
{
    MaterialHandlerPtr gltfMTLXLoader = GltfMaterialHandler::create();
    gltfMTLXLoader->setDefinitions(definitions);
    gltfMTLXLoader->setGenerateAssignments(createAssignments);
    gltfMTLXLoader->setGenerateFullDefinitions(fullDefinition);
    
    bool loadedMaterial = gltfMTLXLoader->load(filename);
    DocumentPtr materials = loadedMaterial ? gltfMTLXLoader->getMaterials() : nullptr;
    
    return materials;
}    

bool GltfMaterialUtil::mtlx2glTF(MaterialHandlerPtr gltfMTLXLoader, 
                                 const FilePath& filename, DocumentPtr materials)
{    
    gltfMTLXLoader->setMaterials(materials);
    return gltfMTLXLoader->save(filename);
}

bool GltfMaterialUtil::haveSingleDocBake(const FilePath& errorFile)
{
    FilePath shaderTranslator(MTLX_TRANSLATE_SHADER);
    if (shaderTranslator.isEmpty())
    {
        return false;
    }

    const string redirectString(" 2>&1");
    string testcommand = "python " + shaderTranslator.asString()
        + " --help"
        +" > " + errorFile.asString() + redirectString;

    int returnValue = system(testcommand.c_str());
    if (returnValue != 0)
    {
        return false;
    }

    std::ifstream errorStream(errorFile.asString());
    string result;
    result.assign(std::istreambuf_iterator<char>(errorStream),
        std::istreambuf_iterator<char>());
    if (std::string::npos != result.find("writeSingleDocument"))
    {
        return true;
    }
    return false;
}

bool GltfMaterialUtil::bakeDocument(const FilePath& inputFileName, const FilePath& outputFilename, 
                     unsigned int width, unsigned int height, std::ostream& logFile)
{
    // Run test renders on output
    FilePath shaderTranslator(MTLX_TRANSLATE_SHADER);
    if (!shaderTranslator.isEmpty())
    {
        const std::string errorFile = outputFilename.asString() + "_errors.txt";
        const std::string redirectString(" 2>&1");

        bool haveSingleBakeDocSupport = GltfMaterialUtil::haveSingleDocBake(errorFile);

        std::string command = "python " + shaderTranslator.asString()
            + " --width " + std::to_string(width)
            + " --height " + std::to_string(height)
            + (haveSingleBakeDocSupport ? " --writeSingleDocument " : EMPTY_STRING)
            + " " + inputFileName.asString()
            + " " + outputFilename.asString()
            + " > " + errorFile + redirectString;

        logFile << "- Translated MTLX: " + outputFilename.asString() << std::endl;
        int returnValue = std::system(command.c_str());

        std::ifstream errorStream(errorFile);
        std::string result;
        result.assign(std::istreambuf_iterator<char>(errorStream),
            std::istreambuf_iterator<char>());

        bool renderError = returnValue != 0 && !result.empty();
        if (renderError)
        {
            StringVec errors;
            logFile << "- Errors: " << std::endl;
            logFile << "  - Command string : " + command << std::endl;
            logFile << "  - Command return code: " + std::to_string(returnValue) << std::endl;
            logFile << "  - Log: " << result << std::endl;
        }
        return true;
        //CHECK(!renderError);
    }
    return false;
}

bool GltfMaterialUtil::renderCheck(const FilePath& materialXViewInstall, const FilePath& fileName, const FilePath& meshFile, 
                                  const std::string& outputFileName,  std::ostream& logFile)
{
    // Run test renders on output
    if (!materialXViewInstall.isEmpty())
    {
        const std::string imageFileName = fileName.asString() + ".png";
        const std::string errorFile = fileName.asString() + "_errors.txt";
        const std::string redirectString(" 2>&1");

        std::string command = materialXViewInstall.asString()
            + " --mesh " + meshFile.asString()
            + " --material " + outputFileName
            + " --screenWidth 512 --screenHeight 512 "
            + " --captureFilename " + imageFileName
            + " --envSampleCount 4"
            + " > " + errorFile + redirectString;

        logFile << "- Render image: " + imageFileName << std::endl;
        int returnValue = std::system(command.c_str());

        std::ifstream errorStream(errorFile);
        std::string result;
        result.assign(std::istreambuf_iterator<char>(errorStream),
            std::istreambuf_iterator<char>());

        bool renderError = returnValue != 0 && !result.empty();
        if (renderError)
        {
            StringVec errors;
            logFile << "- Errors: " << std::endl;
            logFile << "  - Command string : " + command << std::endl;
            logFile << "  - Command return code: " + std::to_string(returnValue) << std::endl;
            logFile << "  - Log: " << result << std::endl;
        }
        return renderError;
    }
    return true;
}

MATERIALX_NAMESPACE_END

