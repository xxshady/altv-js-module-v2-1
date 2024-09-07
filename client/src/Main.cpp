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

ALTV_JSV2_EXPORT alt::IScriptRuntime* JSV2_ENTRY_POINT(alt::ICore* core)
{
    alt::ICore::SetInstance(core);

    CJavaScriptRuntime& runtime = CJavaScriptRuntime::Instance();
    if(!runtime.Initialize()) return nullptr;

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
