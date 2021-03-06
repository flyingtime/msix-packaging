// Win7MSIXInstaller.cpp :
// The main entry point for Win7MSIXInstaller.exe. This application is
// a working preview for the MSIX/APPX installer for Windows 7
#include "MSIXWindows.hpp"
#include <shlobj_core.h>
#include <CommCtrl.h>

#include <string>
#include <map>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <utility>
#include <iomanip>

#include "GeneralUtil.hpp"
#include "FilePaths.hpp"
#include <cstdio>
#include <TraceLoggingProvider.h>
#include "CommandLineInterface.hpp"
#include "resource.h"

// Define the GUID to use in TraceLoggingProviderRegister 
// {033321d3-d599-48e0-868d-c59f15901637}
// One way to enable:
// logman create trace <nameoftrace> -p "{033321d3-d599-48e0-868d-c59f15901637}" -o <filename>
// i.e. logman create trace MsixTrace -p "{033321d3-d599-48e0-868d-c59f15901637}" -o c:\msixtrace.etl
// logman start MsixTrace
// logman stop MsixTrace
// tracerpt.exe, Windows Performance Analyzer or other tools can be used to view the etl file.
TRACELOGGING_DEFINE_PROVIDER(
    g_MsixTraceLoggingProvider,
    "MsixTraceLoggingProvider",
    (0x033321d3, 0xd599, 0x48e0, 0x86, 0x8d, 0xc5, 0x9f, 0x15, 0x90, 0x16, 0x37));

int main(int argc, char * argv[])
{
    // Register the provider
    TraceLoggingRegister(g_MsixTraceLoggingProvider);

    HRESULT hrCoInitialize = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hrCoInitialize))
    {
        std::wcout << GetStringResource(IDS_STRING_FAILED_COM_INITIALIZATION) << " " << std::hex << hrCoInitialize << std::endl;
        return 1;
    }

    CommandLineInterface cli(argc, argv);

    AutoPtr<MsixRequest> msixRequest;
    const HRESULT hrCreateRequest = cli.CreateRequest(&msixRequest);
    if (SUCCEEDED(hrCreateRequest))
    {
        const HRESULT hrProcessRequest = msixRequest->ProcessRequest();
        if (FAILED(hrProcessRequest))
        {

            std::wcout << GetStringResource(IDS_STRING_FAILED_REQUEST) << " " << std::hex << hrProcessRequest << std::endl;
            TraceLoggingWrite(g_MsixTraceLoggingProvider,
                "Failed to process request",
                TraceLoggingLevel(WINEVENT_LEVEL_ERROR),
                TraceLoggingValue(hrProcessRequest, "HR"));
            return 1;
        }
    }
    else
    {
        cli.DisplayHelp();
    }

    
    // Stop TraceLogging and unregister the provider
    TraceLoggingUnregister(g_MsixTraceLoggingProvider);

    return 0;
}

