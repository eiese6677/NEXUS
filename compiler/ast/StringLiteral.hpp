#pragma once

#include "Expression.hpp"
#include <string>

namespace nexus::ast
{

class StringLiteral : public Expression
{
public:
    explicit StringLiteral(std::string value)
        : value(std::move(value))
    {
    }

    const std::string& Value() const
    {
        return value;
    }

    std::string ToString() const override
    {
        return "\"" + value + "\"";
    }

private:
    std::string value;
};

}