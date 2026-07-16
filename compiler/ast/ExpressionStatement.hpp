#pragma once

#include "Statement.hpp"
#include "Expression.hpp"

#include <memory>


namespace nexus::ast
{

class ExpressionStatement : public Statement
{
private:

    std::unique_ptr<Expression> expression;


public:

    explicit ExpressionStatement(std::unique_ptr<Expression> expression)
        : expression(std::move(expression))
    {
    }


    const Expression* GetExpression() const
    {
        return expression.get();
    }


    std::string ToString() const override
    {
        return expression ? expression->ToString() : "ExpressionStatement()";
    }
};

}