#pragma once

#include "Statement.hpp"
#include "Expression.hpp"
#include "BlockStatement.hpp"

#include <memory>
#include <vector>


namespace nexus::ast
{

struct ElseIf
{
    std::unique_ptr<Expression> condition;
    std::unique_ptr<BlockStatement> body;
};

class IfStatement : public Statement
{

private:

    std::unique_ptr<Expression> condition;

    std::unique_ptr<BlockStatement> body;

    std::vector<ElseIf> elifs;

    std::unique_ptr<BlockStatement> elseBody;


public:

    IfStatement(
        std::unique_ptr<Expression> condition,
        std::unique_ptr<BlockStatement> body
    )
        :
        condition(std::move(condition)),
        body(std::move(body))
    {
    }

    void AddElseIf(
        std::unique_ptr<Expression> condition,
        std::unique_ptr<BlockStatement> body
    )
    {
        elifs.push_back(
            {
                std::move(condition),
                std::move(body)
            }
        );
    }

    void SetElse(
        std::unique_ptr<BlockStatement> body
    )
    {
        elseBody = std::move(body);
    }


    std::string ToString() const override
    {
        return "IfStatement";
    }

};

}