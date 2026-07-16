#pragma once

#include "Statement.hpp"

#include <vector>
#include <memory>


namespace nexus::ast
{

class BlockStatement : public Statement
{
private:

    std::vector<std::unique_ptr<Statement>> statements;


public:

    void AddStatement(
        std::unique_ptr<Statement> statement
    )
    {
        statements.push_back(std::move(statement));
    }


    const auto& Statements() const
    {
        return statements;
    }


    std::string ToString() const override
    {
        return "BlockStatement";
    }

};

}