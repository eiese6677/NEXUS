#pragma once

#include "Expression.hpp"

#include <cstdint>

namespace nexus::ast
{

class NumberLiteral : public Expression
{

private:

    std::int64_t value;


public:

    NumberLiteral(std::int64_t value)
        : value(value)
    {
    }


    std::int64_t Value() const
    {
        return value;
    }


    std::string ToString() const override
    {
        return std::to_string(value);
    }

};

}