#include "Logger.h"
#include "interfaces/IResource.h"

js::Logger& js::Logger::operator<<(const js::Logger::EndlStruct&)
{
    alt::IResource* altResource = nullptr;
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    if(isolate != nullptr)
    {
        v8::HandleScope scope(isolate);
        v8::Local<v8::Context> context = isolate->GetEnteredOrMicrotaskContext();
        if(!context.IsEmpty())
        {
            js::IResource* resource = js::IResource::GetFromContext(context);
            if(resource) altResource = resource->GetResource();
        }
    }

    std::string str = stream.str();
    switch(type)
    {
        case Type::INFO: alt::ICore::Instance().LogInfo(PREFIX, str, altResource); break;
        case Type::COLORED: alt::ICore::Instance().LogColored(PREFIX, str, altResource); break;
        case Type::WARN: alt::ICore::Instance().LogWarning(PREFIX, str, altResource); break;
        case Type::ERR: alt::ICore::Instance().LogError(PREFIX, str, altResource); break;
    }
    stream.str("");
    return *this;
}
