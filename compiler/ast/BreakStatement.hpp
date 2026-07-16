#pragma once

#include "Statement.hpp"

namespace nexus::ast
{

class BreakStatement : public Statement
{
public:
    std::string ToString() const override
    {
        return "BreakStatement";
    }
};

}