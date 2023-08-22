
#include <MaterialXglTF/GltfMaterialUtill.h>

#include <fstream>
#include <limits>
#include <unordered_set>

MATERIALX_NAMESPACE_BEGIN

DocumentPtr GltfMaterialUtil::glTF2Mtlx(
                const FilePath& filename, DocumentPtr definitions, 
                bool createAssignments, bool fullDefinition,
                StringVec& logger)
{
    MaterialHandlerPtr gltfMTLXLoader = GltfMaterialHandler::create();
    gltfMTLXLoader->setDefinitions(definitions);
    gltfMTLXLoader->setGenerateAssignments(createAssignments);
    gltfMTLXLoader->setGenerateFullDefinitions(fullDefinition);
    
    bool loadedMaterial = gltfMTLXLoader->load(filename, logger);
    DocumentPtr materials = loadedMaterial ? gltfMTLXLoader->getMaterials() : nullptr;
    
    return materials;
}    

bool GltfMaterialUtil::mtlx2glTF(MaterialHandlerPtr gltfMTLXLoader, 
                                 const FilePath& filename, DocumentPtr materials,
                                 StringVec& logger)
{    
    gltfMTLXLoader->setMaterials(materials);
    return gltfMTLXLoader->save(filename, logger);
}

// Temporary until have build rule for this.
#define MTLX_TRANSLATE_SHADER EMPTY_STRING

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
                                    unsigned int width, unsigned int height, StringVec& logFile)
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

        int returnValue = std::system(command.c_str());

        std::ifstream errorStream(errorFile);
        std::string result;
        result.assign(std::istreambuf_iterator<char>(errorStream),
            std::istreambuf_iterator<char>());

        bool renderError = returnValue != 0 && !result.empty();
        if (renderError)
        {
            StringVec errors;
            logFile.push_back("- Errors: ");
            logFile.push_back("  - Command string : " + command);
            logFile.push_back("  - Command return code: " + std::to_string(returnValue));
            logFile.push_back("  - Log: " + result);
        }
        return renderError;
    }
    return false;
}

bool GltfMaterialUtil::renderImage(const FilePath& materialXViewInstall, const FilePath& captureName, const FilePath& meshFile, 
                                  const std::string& materialFile, StringVec& logFile)
{
    // Run test renders on output
    if (!materialXViewInstall.isEmpty())
    {
        const std::string imageFileName = captureName.asString();
        const std::string errorFile = captureName.getBaseName() + "_errors.txt";
        const std::string redirectString(" 2>&1");

        std::string command = materialXViewInstall.asString();
        if (!meshFile.isEmpty())
            command += " --mesh " + meshFile.asString();
        command
            += " --material " + materialFile
            + " --screenWidth 512 --screenHeight 512 "
            + " --captureFilename " + imageFileName
            + " --envSampleCount 4"
            + " > " + errorFile + redirectString;

        int returnValue = std::system(command.c_str());

        std::ifstream errorStream(errorFile);
        std::string result;
        result.assign(std::istreambuf_iterator<char>(errorStream),
            std::istreambuf_iterator<char>());

        bool renderError = returnValue != 0 && !result.empty();
        if (renderError)
        {
            StringVec errors;
            logFile.push_back("- Errors: ");
            logFile.push_back("  - Command string : " + command);
            logFile.push_back("  - Command return code: " + std::to_string(returnValue));
            logFile.push_back("  - Log: " + result);
        }
        return renderError;
    }
    return true;
}

MATERIALX_NAMESPACE_END

