#pragma once

#include "Statement.hpp"
#include "Expression.hpp"
#include "BlockStatement.hpp"

#include <memory>

namespace nexus::ast
{

class WhileStatement : public Statement
{
private:

    std::unique_ptr<Expression> condition;
    std::unique_ptr<BlockStatement> body;

public:

    WhileStatement(
        std::unique_ptr<Expression> condition,
        std::unique_ptr<BlockStatement> body
    )
        : condition(std::move(condition)),
          body(std::move(body))
    {
    }

    Expression* Condition() const
    {
        return condition.get();
    }

    BlockStatement* Body() const
    {
        return body.get();
    }

    std::string ToString() const override
    {
        return "WhileStatement";
    }
};

}