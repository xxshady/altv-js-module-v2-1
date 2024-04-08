#include "Class.h"

static void Constructor(js::FunctionContext& ctx)
{
    if (!ctx.CheckCtor()) return;
    if (!ctx.CheckArgCount(0, 2)) return;

    std::string encoding = "utf-8";
    if (ctx.GetArgCount() > 0)
    {
        if (!ctx.GetArg(0, encoding)) return;
        if (!ctx.Check(encoding == "utf-8", "Invalid encoding")) return;
    }

    bool fatal = false;
    bool ignoreBOM = false;
    if(ctx.GetArgCount() > 1)
    {
        js::Object options;
        if (!ctx.GetArg(1, options)) return;

        fatal = options.Get<bool>("fatal");
        ignoreBOM = options.Get<bool>("ignoreBOM");
    }

    ctx.GetThis()->Set(ctx.GetContext(), js::JSValue("__encoding"), js::JSValue(encoding));
    ctx.GetThis()->Set(ctx.GetContext(), js::JSValue("__fatal"), js::JSValue(fatal));
    ctx.GetThis()->Set(ctx.GetContext(), js::JSValue("__ignoreBOM"), js::JSValue(ignoreBOM));
}

static void EncodingGetter(js::PropertyContext& ctx)
{
    if (!ctx.CheckThis()) return;

    ctx.Return(ctx.GetThis()->Get(ctx.GetContext(), js::JSValue("__encoding")).ToLocalChecked());
}

extern js::Class textDecoderClass("TextDecoder", Constructor, [](js::ClassTemplate& tpl)
{
    tpl.Property("encoding", EncodingGetter);
});
