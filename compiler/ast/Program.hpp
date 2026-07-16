#pragma once

#include "Statement.hpp"

#include <memory>
#include <vector>


namespace nexus::ast
{

class Program : public Node
{
private:

    std::vector<std::unique_ptr<Statement>> statements;


public:

    void AddStatement(std::unique_ptr<Statement> statement)
    {
        if (statement)
        {
            statements.push_back(std::move(statement));
        }
    }


    const auto& Statements() const
    {
        return statements;
    }


    std::string ToString() const override
    {
        return "Program";
    }
};

}