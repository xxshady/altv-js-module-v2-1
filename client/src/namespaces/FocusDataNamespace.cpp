#include "Namespace.h"

static void FocusOverridenGetter(js::PropertyContext& ctx)
{
    ctx.Return(alt::ICore::Instance().IsFocusOverriden());
}

static void FocusOverridePosGetter(js::PropertyContext& ctx)
{
    ctx.Return(alt::ICore::Instance().GetFocusOverridePos());
}

static void OverrideFocus(js::FunctionContext& ctx)
{
    if (!ctx.CheckArgCount(1, 2)) return;

    alt::Position pos;
    if (!ctx.GetArg(0, pos)) return;

    auto offset = ctx.GetArg<alt::Vector3f>(1, { 0, 0, 0 });

    alt::ICore::Instance().OverrideFocusPosition(pos, offset);
}

static void FocusOverrideEntityGetter(js::PropertyContext& ctx)
{
    ctx.Return(alt::ICore::Instance().GetFocusOverrideEntity());
}

static void FocusOverrideEntitySetter(js::PropertyContext& ctx)
{
    alt::IEntity* entity;
    if(!ctx.GetValue(entity)) return;

    alt::ICore::Instance().OverrideFocusEntity(entity);
}

static void FocusOverrideOffsetGetter(js::PropertyContext& ctx)
{
    ctx.Return(alt::ICore::Instance().GetFocusOverrideOffset());
}

static void ClearFocusOverride(js::FunctionContext& ctx)
{
    alt::ICore::Instance().ClearFocusOverride();
}

// clang-format off
extern js::Namespace focusDataNamespace("FocusData", [](js::NamespaceTemplate& tpl) {
    tpl.StaticProperty("isFocusOverriden", FocusOverridenGetter);
    tpl.StaticProperty("focusOverridePos", FocusOverridePosGetter);
    tpl.StaticProperty("focusOverrideEntity", FocusOverrideEntityGetter, FocusOverrideEntitySetter);
    tpl.StaticProperty("focusOverrideOffset", FocusOverrideOffsetGetter);


    tpl.StaticFunction("overrideFocus", OverrideFocus);
    tpl.StaticFunction("clearFocusOverride", ClearFocusOverride);
});
