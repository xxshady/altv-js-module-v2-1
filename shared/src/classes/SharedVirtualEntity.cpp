#include "Class.h"
#include "cpp-sdk/ICore.h"

static void SyncedMetaGetter(js::DynamicPropertyGetterContext& ctx)
{
    if(!ctx.CheckParent()) return;
    alt::IVirtualEntity* obj = ctx.GetParent<alt::IVirtualEntity>();
    ctx.Return(obj->GetSyncedMetaData(ctx.GetProperty()));
}

static void SyncedMetaEnumerator(js::DynamicPropertyEnumeratorContext& ctx)
{
    if(!ctx.CheckParent()) return;
    alt::IVirtualEntity* obj = ctx.GetParent<alt::IVirtualEntity>();
    ctx.Return(obj->GetSyncedMetaDataKeys());
}

static void StreamSyncedMetaGetter(js::DynamicPropertyGetterContext& ctx)
{
    if(!ctx.CheckParent()) return;
    alt::IVirtualEntity* obj = ctx.GetParent<alt::IVirtualEntity>();
    ctx.Return(obj->GetStreamSyncedMetaData(ctx.GetProperty()));
}

static void StreamSyncedMetaEnumerator(js::DynamicPropertyEnumeratorContext& ctx)
{
    if(!ctx.CheckParent()) return;
    alt::IVirtualEntity* obj = ctx.GetParent<alt::IVirtualEntity>();
    ctx.Return(obj->GetStreamSyncedMetaDataKeys());
}

// clang-format off
extern js::Class worldObjectClass;
extern js::Class sharedVirtualEntityClass("SharedVirtualEntity", &worldObjectClass, nullptr, [](js::ClassTemplate& tpl)
{
    tpl.LazyProperty<&alt::IVirtualEntity::GetGroup>("group");
    tpl.LazyProperty<&alt::IVirtualEntity::GetStreamingDistance>("streamingDistance");

    tpl.Property<&alt::IVirtualEntity::IsVisible, &alt::IVirtualEntity::SetVisible>("visible");

    // tpl.DynamicProperty("syncedMeta", SyncedMetaGetter, nullptr, nullptr, SyncedMetaEnumerator);
    tpl.DynamicProperty("streamSyncedMeta", StreamSyncedMetaGetter, nullptr, nullptr, StreamSyncedMetaEnumerator);
});
