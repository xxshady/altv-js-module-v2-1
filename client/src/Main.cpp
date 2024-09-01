#include "cpp-sdk/version/version.h"
#include "cpp-sdk/SDK.h"

#include "ClientCommandHandlers.h"
#include "CommandHandlers.h"
#include "CJavaScriptRuntime.h"

#ifdef ALTV_JSV2_SHARED
    #define ALTV_JSV2_EXPORT extern "C" __declspec(dllexport)
    #define JSV2_ENTRY_POINT CreateScriptRuntime
#else
    #define ALTV_JSV2_EXPORT extern "C"
    #define JSV2_ENTRY_POINT CreateScriptRuntimeJSv2
#endif

#ifndef ALTV_JSV2_SHARED
extern js::Module altModule;
extern js::Module compatibilityModule;
extern js::Module compatibilityNativesModule;
extern js::Module sharedCppBindingsModule;
extern js::Module cppBindingsModule;
extern js::Module nativesModule;
extern js::Module sharedCompatibilityModule;
extern js::Module sharedCompatibilityModule2;
extern js::Module sharedModule;

// Workaround to ensure linker doesnt exclude them from build
static void KeepModules()
{
    static std::vector<js::Module*> modules;
    modules.push_back(&altModule);
    modules.push_back(&compatibilityModule);
    modules.push_back(&compatibilityNativesModule);
    modules.push_back(&sharedCppBindingsModule);
    modules.push_back(&cppBindingsModule);
    modules.push_back(&nativesModule);
    modules.push_back(&sharedCompatibilityModule);
    modules.push_back(&sharedCompatibilityModule2);
    modules.push_back(&sharedModule);
}
#endif

ALTV_JSV2_EXPORT alt::IScriptRuntime* JSV2_ENTRY_POINT(alt::ICore* core)
{
    alt::ICore::SetInstance(core);

    CJavaScriptRuntime& runtime = CJavaScriptRuntime::Instance();
    if(!runtime.Initialize()) return nullptr;

#ifndef ALTV_JSV2_SHARED
    KeepModules();
#endif

    core->SubscribeCommand("debughandles", js::DebugHandlesCommand);
    core->SubscribeCommand("dumpbinding", js::DumpBindingCommand);
    core->SubscribeCommand("dumpallbindings", js::DumpAllBindingsCommand);
    core->SubscribeCommand("dumpmodulescache", js::DumpModulesCacheCommand);
    core->SubscribeCommand("dumpsample", js::DumpSampleCommand);
    core->SubscribeCommand("dumpallsamples", js::DumpAllSamplesCommand);
    core->SubscribeCommand("resetsamples", js::ResetSamplesCommand);
    core->SubscribeCommand("dumpbuffers", js::DumpBuffersCommand);
    core->SubscribeCommand("dumpheap", js::DumpHeapCommand);

    return &runtime;
}

#ifdef ALTV_JSV2_SHARED
ALTV_JSV2_EXPORT const char* GetType()
{
    return "jsv2";
}

ALTV_JSV2_EXPORT const char* GetSDKHash()
{
    return ALT_SDK_VERSION;
}
#endif
