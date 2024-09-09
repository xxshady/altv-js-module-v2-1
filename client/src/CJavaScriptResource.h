#pragma once

#include "interfaces/IAltResource.h"
#include "helpers/JS.h"
#include "helpers/IModuleHandler.h"
#include "helpers/IExceptionHandler.h"

#include "v8.h"

class CJavaScriptRuntime;

class CJavaScriptResource : public js::IAltResource, public IModuleHandler, public IExceptionHandler
{
    friend class CJavaScriptRuntime;

    std::unique_ptr<v8::MicrotaskQueue> microtaskQueue;
    bool started = false;
    std::shared_ptr<alt::INative::Context> nativeContext;

    v8::Local<v8::Module> CompileAndRun(const std::string& path, const std::string& source);

    void LoadConfig();

public:
    CJavaScriptResource(alt::IResource* resource, v8::Isolate* isolate) : IAltResource(resource, isolate) {}

    bool Start() override;
    bool Stop() override;

    void OnEvent(const alt::CEvent* ev) override;
    void OnTick() override;

    void RunEventLoop() override;

    std::shared_ptr<alt::INative::Context> GetNativeContext()
    {
        return nativeContext;
    }
};
