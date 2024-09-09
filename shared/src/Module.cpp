#include "Module.h"
#include "Class.h"
#include "interfaces/IResource.h"

void js::Module::Register(ModuleTemplate& tpl)
{
    v8::Isolate* isolate = tpl.GetIsolate();
    if(!parentModule.empty())
    {
        Module& parentMod = Get(parentModule);
        parentMod.Register(tpl);
    }
    for(js::Class* class_ : classes)
    {
        tpl.StaticProperty(class_->GetName(), class_->GetTemplate(isolate).Get());
    }
    if(initCb) initCb(tpl);
}

v8::Local<v8::Object> js::Module::GetNamespace(IResource* resource)
{
    js::Object modNamespace(nullptr);
    if(customGetNamespaceCb) modNamespace = customGetNamespaceCb(resource);
    else
    {
        if(!instanceMap.contains(resource))
        {
            v8::Local<v8::Object> obj = templateMap.at(resource->GetIsolate()).Get()->NewInstance(resource->GetContext()).ToLocalChecked();
            instanceMap.insert({ resource, Persistent<v8::Object>(resource->GetIsolate(), obj) });
            modNamespace = obj;
        }
        else
            modNamespace = instanceMap.at(resource).Get(resource->GetIsolate());
    }

    if(HasOption(Option::EXPORT_AS_DEFAULT)) modNamespace.Set("default", modNamespace.Get());
    return modNamespace.Get();
}

void js::Module::Initialize(v8::Isolate* isolate)
{
    for(auto& [name, module] : GetAll())
    {
        ModuleTemplate mod{ isolate, v8::ObjectTemplate::New(isolate) };
        module->Register(mod);
        module->templateMap.insert({ isolate, mod });
    }
}

void js::Module::Cleanup(v8::Isolate* isolate)
{
    for(auto& [_, module] : GetAll()) module->templateMap.erase(isolate);
}

void js::Module::CleanupForResource(IResource* resource)
{
    for(auto& [_, mod] : GetAll()) mod->instanceMap.erase(resource);
}
