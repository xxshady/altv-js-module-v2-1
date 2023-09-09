﻿#include "Class.h"
#include "interfaces/IResource.h"
#include "cpp-sdk/ICore.h"

static void Accept(js::FunctionContext& ctx)
{
    if (!ctx.CheckThis()) return;

    auto* connectionInfo = ctx.GetThisObject<alt::IConnectionInfo>();
    bool sendNames = ctx.GetArg(0, true);

    connectionInfo->Accept(sendNames);
}

// clang-format off
extern js::Class connectionInfoClass("ConnectionInfo", nullptr, nullptr, [](js::ClassTemplate& tpl)
{
    tpl.BindToType(alt::IBaseObject::Type::CONNECTION_INFO);

    tpl.LazyProperty<&alt::IConnectionInfo::GetName>("name");
    tpl.LazyProperty<&alt::IConnectionInfo::GetSocialId>("socialId");
    tpl.LazyProperty<&alt::IConnectionInfo::GetSocialName>("socialName");
    tpl.LazyProperty<&alt::IConnectionInfo::GetHwIdHash>("hwidHash");
    tpl.LazyProperty<&alt::IConnectionInfo::GetHwIdExHash>("hwidExHash");
    tpl.LazyProperty<&alt::IConnectionInfo::GetAuthToken>("authToken");
    tpl.LazyProperty<&alt::IConnectionInfo::GetIsDebug>("debug");
    tpl.LazyProperty<&alt::IConnectionInfo::GetBranch>("branch");
    tpl.LazyProperty<&alt::IConnectionInfo::GetBuild>("build");
    tpl.LazyProperty<&alt::IConnectionInfo::GetCdnUrl>("cdnUrl");
    tpl.LazyProperty<&alt::IConnectionInfo::GetPasswordHash>("passwordHash");
    tpl.LazyProperty<&alt::IConnectionInfo::GetIp>("ip");
    tpl.LazyProperty<&alt::IConnectionInfo::GetDiscordUserID>("discordUserId");
    tpl.LazyProperty<&alt::IConnectionInfo::GetCloudAuthHash>("cloudAuthHash");

    tpl.Property<&alt::IConnectionInfo::IsAccepted>("isAccepted");
    tpl.Property<&alt::IConnectionInfo::GetText, &alt::IConnectionInfo::SetText>("text");

    tpl.Method("accept", Accept);
    tpl.Method<&alt::IConnectionInfo::Decline>("decline");
});