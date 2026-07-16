#pragma once

#include "Statement.hpp"

namespace nexus::ast
{

class ContinueStatement : public Statement
{
public:
    std::string ToString() const override
    {
        return "ContinueStatement";
    }
};

}