#include "Class.h"
#include "interfaces/IResource.h"

static void DefaultCtor(js::FunctionContext& ctx)
{
    if(ctx.GetArgCount() != 0) ctx.Throw("Use alt.*.create() to create an instance of this class");
}

void js::Class::Register(v8::Isolate* isolate)
{
    ClassTemplate tpl{ isolate, this, ctor ? WrapFunction(ctor) : WrapFunction(DefaultCtor) };
    if(parentClass)
    {
        if(!parentClass->templateMap.contains(isolate)) parentClass->Register(isolate);
        tpl.Inherit(parentClass->templateMap.at(isolate));
    }
    initCb(tpl);
    tpl.RegisterStaticMethods();
    tpl.Get()->SetClassName(js::JSValue(name));
    tpl.Get()->InstanceTemplate()->SetInternalFieldCount(internalFieldCount);
    templateMap.insert({ isolate, tpl });
}

js::Class* js::Class::GetByClassId(uint16_t id)
{
    for(auto class_ : GetAll())
    {
        if(class_->classId == id) return class_;
    }
    return nullptr;
}

void js::Class::Initialize(v8::Isolate* isolate)
{
    for(auto class_ : GetAll()) class_->Register(isolate);

    // Only needed while setting up the templates, so we can free the data here
    ClassTemplate::CleanupPropertyGetterMap(isolate);
}

void js::Class::Cleanup(v8::Isolate* isolate)
{
    for(auto class_ : GetAll()) class_->templateMap.erase(isolate);
    ClassTemplate::CleanupDynamicPropertyData(isolate);
}
