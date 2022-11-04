//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#define CATCH_CONFIG_RUNNER

#if defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4996)
#endif
#include <glTFMtlxTest/Catch/catch.hpp>
#if defined(_MSC_VER)
    #pragma warning(pop)
#endif

int main(int argc, char* const argv[])
{
    Catch::Session session;

#ifndef _DEBUG
    session.configData().showDurations = Catch::ShowDurations::Always;
#endif
#ifdef CATCH_PLATFORM_WINDOWS
    BOOL inDebugger = IsDebuggerPresent();
    if (inDebugger)
    {
        session.configData().outputFilename = "%debug";
    }
    else
    {
        session.configData().outputFilename = "";
    }
#endif

    int returnCode = session.applyCommandLine(argc, argv);
    if (returnCode != 0)
    {
        return returnCode;
    }

    return session.run();
}
