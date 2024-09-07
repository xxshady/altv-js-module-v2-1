#include "Module.h"
#include "interfaces/IResource.h"

// clang-format off
static js::Module sharedCompatibilityModule("alt", [](js::IResource* resource) -> v8::Local<v8::Object> { return resource->GetCompatibilityModuleNamespace(); }, js::Module::Option::COMPATIBILITY_MODULE | js::Module::Option::EXPORT_AS_DEFAULT);
static js::Module sharedCompatibilityModule2("alt-shared", [](js::IResource* resource) -> v8::Local<v8::Object> { return resource->GetCompatibilityModuleNamespace(); }, js::Module::Option::COMPATIBILITY_MODULE | js::Module::Option::EXPORT_AS_DEFAULT);
