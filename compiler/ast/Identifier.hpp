#pragma once

#include "Expression.hpp"

namespace nexus::ast
{

class Identifier : public Expression
{
private:
    std::string name;


public:

    Identifier(const std::string& name)
        : name(name)
    {
    }


    const std::string& Name() const
    {
        return name;
    }


    std::string ToString() const override
    {
        return name;
    }
};

}