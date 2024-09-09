#pragma once

#include "v8.h"
#include "cpp-sdk/SDK.h"

#include "helpers/Buffer.h"

#include <sstream>

namespace js
{
    class IResource;
    enum class Type : uint8_t
    {
        INVALID,
        UNDEFINED,
        NULL_TYPE,
        BOOLEAN,
        NUMBER,
        STRING,
        SYMBOL,
        OBJECT,
        FUNCTION,
        ARRAY,
        ARRAY_BUFFER,
        ARRAY_BUFFER_VIEW,
        SHARED_ARRAY_BUFFER,
        TYPED_ARRAY,
        DATA_VIEW,
        DATE,
        PROMISE,
        MAP,
        SET,
        BIG_INT,

        // Custom types
        VECTOR3,
        VECTOR2,
        RGBA,
        QUATERNION,
        BASE_OBJECT,
        BUFFER
    };

    bool IsVector3(v8::Local<v8::Value> value, IResource* resource);
    bool IsVector2(v8::Local<v8::Value> value, IResource* resource);
    bool IsRGBA(v8::Local<v8::Value> value, IResource* resource);
    bool IsQuaternion(v8::Local<v8::Value> value, IResource* resource);
    bool IsBaseObject(v8::Local<v8::Value> value, IResource* resource);
    bool IsBuffer(v8::Local<v8::Value> value, IResource* resource);

    static Type GetType(v8::Local<v8::Value> value, js::IResource* resource)
    {
        if(value.IsEmpty()) return Type::INVALID;
        if(value->IsUndefined()) return Type::UNDEFINED;
        if(value->IsNull()) return Type::NULL_TYPE;
        if(value->IsBoolean()) return Type::BOOLEAN;
        if(value->IsNumber()) return Type::NUMBER;
        if(value->IsString()) return Type::STRING;
        if(value->IsSymbol()) return Type::SYMBOL;
        if(value->IsObject())
        {
            if(value->IsFunction()) return Type::FUNCTION;
            if(value->IsArray()) return Type::ARRAY;
            if(value->IsArrayBuffer()) return Type::ARRAY_BUFFER;
            if(value->IsArrayBufferView()) return Type::ARRAY_BUFFER_VIEW;
            if(value->IsSharedArrayBuffer()) return Type::SHARED_ARRAY_BUFFER;
            if(value->IsTypedArray()) return Type::TYPED_ARRAY;
            if(value->IsDataView()) return Type::DATA_VIEW;
            if(value->IsDate()) return Type::DATE;
            if(value->IsPromise()) return Type::PROMISE;
            if(value->IsMap()) return Type::MAP;
            if(value->IsSet()) return Type::SET;
            if(value->IsBigInt()) return Type::BIG_INT;

            if(resource)
            {
                if(IsVector3(value, resource)) return Type::VECTOR3;
                if(IsVector2(value, resource)) return Type::VECTOR2;
                if(IsRGBA(value, resource)) return Type::RGBA;
                if(IsQuaternion(value, resource)) return Type::QUATERNION;
                if(IsBaseObject(value, resource)) return Type::BASE_OBJECT;
                if(IsBuffer(value, resource)) return Type::BUFFER;
            }

            return Type::OBJECT;
        }
        return Type::INVALID;
    }

    static std::string TypeToString(Type type)
    {
        switch(type)
        {
            case Type::UNDEFINED: return "undefined";
            case Type::NULL_TYPE: return "null";
            case Type::BOOLEAN: return "boolean";
            case Type::NUMBER: return "number";
            case Type::STRING: return "string";
            case Type::SYMBOL: return "symbol";
            case Type::OBJECT: return "object";
            case Type::FUNCTION: return "function";
            case Type::ARRAY: return "array";
            case Type::ARRAY_BUFFER: return "ArrayBuffer";
            case Type::ARRAY_BUFFER_VIEW: return "ArrayBufferView";
            case Type::SHARED_ARRAY_BUFFER: return "SharedArrayBuffer";
            case Type::TYPED_ARRAY: return "TypedArray";
            case Type::DATA_VIEW: return "DataView";
            case Type::DATE: return "Date";
            case Type::PROMISE: return "Promise";
            case Type::MAP: return "Map";
            case Type::SET: return "Set";
            case Type::BIG_INT: return "BigInt";
            case Type::VECTOR3: return "Vector3";
            case Type::VECTOR2: return "Vector2";
            case Type::RGBA: return "RGBA";
            case Type::QUATERNION: return "Quaternion";
            case Type::BASE_OBJECT: return "BaseObject";
            case Type::BUFFER: return "Buffer";
        }
        return "Invalid";
    }

    static std::string TypeToString(std::initializer_list<Type> types)
    {
        std::stringstream typesString;
        for(size_t i = 0; i < types.size(); i++)
        {
            typesString << TypeToString(types.begin()[i]);
            if(i != types.size() - 1) typesString << ", ";
        }
        return typesString.str();
    }

    template<typename T>
    static constexpr Type CppTypeToJSType()
    {
        // clang-format off
        if constexpr(std::is_same_v<T, nullptr_t>) return Type::NULL_TYPE;
        if constexpr(std::is_same_v<T, bool>) return Type::BOOLEAN;
        if constexpr((std::is_integral_v<T> || std::is_floating_point_v<T>) && sizeof(T) < 8) return Type::NUMBER;
        if constexpr((std::is_integral_v<T> || std::is_floating_point_v<T>) && sizeof(T) >= 8) return Type::BIG_INT;
        if constexpr(std::is_same_v<T, std::string>) return Type::STRING;
        if constexpr(std::is_same_v<T, alt::Vector3f> || std::is_same_v<T, alt::Position> || std::is_same_v<T, alt::Rotation>) return Type::VECTOR3;
        if constexpr(std::is_same_v<T, alt::Vector2f>) return Type::VECTOR2;
        if constexpr(std::is_same_v<T, alt::RGBA>) return Type::RGBA;
        if constexpr(std::is_same_v<T, alt::Quaternion>) return Type::QUATERNION;
        if constexpr(std::is_base_of_v<alt::IBaseObject, std::remove_pointer_t<T>>) return Type::BASE_OBJECT;
        if constexpr(std::is_same_v<std::remove_pointer_t<T>, js::Buffer>) return Type::BUFFER;
        return Type::INVALID;
        // clang-format on
    }

    template<typename T>
    static constexpr std::string CppTypeToString()
    {
        return TypeToString(CppTypeToJSType<T>());
    }
}  // namespace js
