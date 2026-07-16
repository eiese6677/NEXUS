#pragma once

#include "Expression.hpp"

#include <vector>
#include <memory>


namespace nexus::ast
{

class CallExpression : public Expression
{
private:

    std::string name;

    std::vector<std::unique_ptr<Expression>> arguments;


public:

    CallExpression(
        const std::string& name
    )
        : name(name)
    {
    }


    void AddArgument(
        std::unique_ptr<Expression> expr
    )
    {
        arguments.push_back(std::move(expr));
    }


    std::string ToString() const override
    {
        return "Call(" + name + ")";
    }
};

}