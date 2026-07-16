#pragma once

#include "Statement.hpp"
#include "Expression.hpp"

#include <memory>

namespace nexus::ast
{

class ReturnStatement : public Statement
{
private:

    std::unique_ptr<Expression> value;

public:

    explicit ReturnStatement(std::unique_ptr<Expression> value)
        : value(std::move(value))
    {
    }

    Expression* Value() const
    {
        return value.get();
    }

    std::string ToString() const override
    {
        return "ReturnStatement";
    }
};

}