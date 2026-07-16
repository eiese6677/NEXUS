#pragma once

#include "Expression.hpp"

namespace nexus::ast
{

class FloatLiteral : public Expression
{

private:

    double value;


public:

    FloatLiteral(double value)
        : value(value)
    {
    }


    double Value() const
    {
        return value;
    }


    std::string ToString() const override
    {
        return std::to_string(value);
    }

};

}