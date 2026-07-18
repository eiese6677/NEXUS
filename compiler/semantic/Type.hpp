#pragma once

#include <string>

namespace nexus::semantic
{

enum class Type
{
    Unknown,

    Int,
    Float,
    String,
    Bool,

    Void
};

inline std::string TypeToString(Type type)
{
    switch (type)
    {
    case Type::Unknown:
        return "Unknown";
        break;
    case Type::Int:
        return "Int";
        break;
    case Type::Float:
        return "Float";
        break;
    case Type::String:
        return "String";
        break;
    case Type::Bool:
        return "Bool";
        break;
    case Type::Void:
        return "Void";
        break;
    default:
        break;
    }
};

}