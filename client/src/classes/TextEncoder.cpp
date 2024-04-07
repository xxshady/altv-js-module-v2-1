#include "Class.h"

static void EncodingGetter(js::LazyPropertyContext& ctx)
{
    ctx.Return(std::string("utf-8"));
}


static void Encode(js::FunctionContext& ctx)
{
    if (!ctx.CheckArgCount(1)) return;

    std::string str;
    if (!ctx.GetArg(0, str)) return;

    size_t size = str.size();
    v8::Local<v8::ArrayBuffer> buffer = v8::ArrayBuffer::New(ctx.GetIsolate(), size);
    std::shared_ptr<v8::BackingStore> backingStore = buffer->GetBackingStore();
    memcpy(backingStore->Data(), str.data(), size);

    ctx.Return(v8::Uint8Array::New(buffer, 0, size));
}

static void EncodeInto(js::FunctionContext& ctx)
{
    if (!ctx.CheckArgCount(2)) return;

    v8::Local<v8::Value> strValue;
    if (!ctx.GetArg(0, strValue)) return;
    if (!ctx.Check(strValue->IsString(), "Argument 0 must be a string")) return;

    v8::Local<v8::Value> arrValue;
    if (!ctx.GetArg(1, arrValue)) return;
    if (!ctx.Check(arrValue->IsUint8Array(), "Argument 1 must be a Uint8Array")) return;

    v8::Local<v8::String> str = strValue.As<v8::String>();
    v8::Local<v8::Uint8Array> uint8Array = arrValue.As<v8::Uint8Array>();

    v8::Local<v8::ArrayBuffer> buffer = uint8Array->Buffer();
    char* data = (char*)((uintptr_t)buffer->GetBackingStore()->Data() + uint8Array->ByteOffset());

    size_t length = uint8Array->ByteLength();
    size_t writeSize = str->WriteUtf8(ctx.GetIsolate(), data, length, nullptr, v8::String::NO_NULL_TERMINATION | v8::String::REPLACE_INVALID_UTF8);

    js::Object result;
    result.Set("read", writeSize);
    result.Set("written", writeSize);
    ctx.Return(result);
}

extern js::Class textEncoderClass("TextEncoder", [](js::ClassTemplate& tpl)
{
    tpl.LazyProperty("encoding", EncodingGetter);

    tpl.Method("encode", Encode);
    tpl.Method("encodeInto", EncodeInto);
});
