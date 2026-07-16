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

    void AddArgument(std::unique_ptr<Expression> argument)
    {
        arguments.push_back(std::move(argument));
    }

    const std::string& Name() const
    {
        return name;
    }

    const std::vector<std::unique_ptr<Expression>>& Arguments() const
    {
        return arguments;
    }

    std::string ToString() const override
    {
        return "Call(" + name + ")";
    }
};

}