#include "CJavaScriptRuntime.h"
#include "libplatform/libplatform.h"
#include "Logger.h"

void CJavaScriptRuntime::OnFatalError(const char* location, const char* message)
{
    js::Logger::Error("[JS] V8 fatal error!", location, message);
}

void CJavaScriptRuntime::OnHeapOOM(const char* location, bool isHeap)
{
    if(!isHeap) return;
    js::Logger::Error("[JS] V8 heap out of memory!", location);
}

size_t CJavaScriptRuntime::OnNearHeapLimit(void*, size_t current, size_t initial)
{
    js::Logger::Warn("[JS] The remaining V8 heap space is approaching critical levels. Increasing heap limit...");

    // Increase the heap limit by 100MB if the heap limit has not exceeded 4GB
    uint64_t currentLimitMb = (current / 1024) / 1024;
    if(currentLimitMb < 4096) return current + (100 * 1024 * 1024);
    else
        return current;
}

void CJavaScriptRuntime::OnPromiseRejected(v8::PromiseRejectMessage message)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();

    CJavaScriptResource* resource = js::IResource::GetFromContext<CJavaScriptResource>(ctx);
    if(!resource) return;
    resource->OnPromiseRejected(message);
}

v8::MaybeLocal<v8::Promise> CJavaScriptRuntime::ImportModuleDynamically(
  v8::Local<v8::Context> context, v8::Local<v8::Data> hostDefinedOptions, v8::Local<v8::Value> resourceName, v8::Local<v8::String> specifier, v8::Local<v8::FixedArray> importAssertions)
{
    CJavaScriptResource* resource = js::IResource::GetFromContext<CJavaScriptResource>(context);
    js::Promise promise;
    std::string referrer = js::CppValue(resourceName.As<v8::String>());
    v8::Local<v8::Module> referrerModule = resource->GetModuleFromPath(referrer);
    if(referrerModule.IsEmpty())
    {
        promise.Reject("Could not find referrer module");
        return promise.Get();
    }

    std::unordered_map<std::string, std::string> assertions = IModuleHandler::TransformImportAssertions(importAssertions);
    v8::MaybeLocal<v8::Module> maybeModule = resource->Resolve(context, js::CppValue(specifier), referrerModule, assertions);
    if(maybeModule.IsEmpty())
    {
        promise.Reject("Could not resolve module");
        return promise.Get();
    }

    v8::Local<v8::Module> module = maybeModule.ToLocalChecked();
    if(module->GetStatus() == v8::Module::Status::kUninstantiated && !resource->InstantiateModule(context, module))
    {
        promise.Reject("Failed to instantiate module");
        return promise.Get();
    }

    if(module->GetStatus() != v8::Module::Status::kEvaluated)
    {
        resource->EvaluateModule(context, module);
        if(module->GetStatus() == v8::Module::Status::kErrored)
        {
            promise.Reject("Failed to evaluate module");
            return promise.Get();
        }
    }

    promise.Resolve(module->GetModuleNamespace());
    return promise.Get();
}

static void Resolve(js::FunctionContext& ctx)
{
    if(!ctx.CheckArgCount(1)) return;

    std::string path;
    if(!ctx.GetArg(0, path)) return;

    js::IAltResource* resource = ctx.GetResource<js::IAltResource>();
    std::string currentFile = js::SourceLocation::GetCurrent(resource).file;
    alt::IPackage::PathInfo pathInfo = alt::ICore::Instance().Resolve(resource->GetResource(), path, currentFile);
    if(!ctx.Check(pathInfo.pkg, "Invalid path")) return;

    ctx.Return(pathInfo.prefix + pathInfo.fileName);
}

void CJavaScriptRuntime::InitializeImportMetaObject(v8::Local<v8::Context> context, v8::Local<v8::Module> mod, v8::Local<v8::Object> meta)
{
    CJavaScriptResource* resource = js::IResource::GetFromContext<CJavaScriptResource>(context);
    if(!resource) return;

    js::Object metaObj(meta);
    metaObj.Set("url", resource->GetModulePath(mod));
    metaObj.SetMethod("resolve", Resolve);
}

void CJavaScriptRuntime::MessageListener(v8::Local<v8::Message> message, v8::Local<v8::Value> error)
{
    js::Logger::Warn("[JS] V8 message received!", js::CppValue(message->Get()));
}

void CJavaScriptRuntime::SetupIsolateHandlers()
{
    isolate->SetFatalErrorHandler(OnFatalError);
    isolate->SetOOMErrorHandler(OnHeapOOM);
    isolate->AddNearHeapLimitCallback(OnNearHeapLimit, nullptr);
    isolate->SetPromiseRejectCallback(OnPromiseRejected);
    isolate->SetHostImportModuleDynamicallyCallback(ImportModuleDynamically);
    isolate->SetHostInitializeImportMetaObjectCallback(InitializeImportMetaObject);
    isolate->AddMessageListener(MessageListener);
}

bool CJavaScriptRuntime::Initialize()
{
    v8::V8::SetFlagsFromString("--harmony-import-assertions --short-builtin-calls --no-lazy --no-flush-bytecode");

#ifdef ALTV_JSV2_SHARED
    platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::InitializeICU((alt::ICore::Instance().GetClientPath() + "/libs/icudtl_v8.dat").c_str());
    v8::V8::Initialize();
#endif  // ALTV_JSV2_SHARED

    v8::Isolate::CreateParams createParams;
    createParams.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    createParams.allow_atomics_wait = false;

    isolate = v8::Isolate::New(createParams);
    isolate->SetMicrotasksPolicy(v8::MicrotasksPolicy::kExplicit);
    isolate->SetCaptureStackTraceForUncaughtExceptions(true, 5);
    SetupIsolateHandlers();

    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);
    return IRuntime::Initialize();
}

void CJavaScriptRuntime::OnTick()
{
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    v8::platform::PumpMessageLoop(GetPlatform(), isolate);
}

void CJavaScriptRuntime::OnDispose()
{
    while(isolate->IsInUse()) isolate->Exit();
    v8::platform::NotifyIsolateShutdown(GetPlatform(), isolate);
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::DisposePlatform();

    delete isolate->GetArrayBufferAllocator();
}
