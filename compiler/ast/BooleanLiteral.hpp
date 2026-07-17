#pragma once

#include "Expression.hpp"

namespace nexus::ast
{

class BooleanLiteral : public Expression
{
private:
    bool value;

public:
    explicit BooleanLiteral(bool value)
        : value(value)
    {
    }

    bool Value() const
    {
        return value;
    }

    std::string ToString() const override
    {
        return value ? "참" : "거짓";
    }
};

}