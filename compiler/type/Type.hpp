#pragma once

namespace nexus::type
{

enum class TypeKind
{
    Int,
    Float,
    String,
    Bool,
    Unknown
};

struct Type
{
    TypeKind kind;

    bool operator==(const Type& other) const
    {
        return kind == other.kind;
    }
};

}