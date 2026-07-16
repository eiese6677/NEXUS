#pragma once

#include "Statement.hpp"
#include "Identifier.hpp"
#include "Expression.hpp"

#include <memory>


namespace nexus::ast
{

class VariableDeclaration : public Statement
{

private:

    std::string typeName;

    std::unique_ptr<Identifier> name;

    std::unique_ptr<Expression> value;


public:

    VariableDeclaration(
        const std::string& typeName,
        std::unique_ptr<Identifier> name,
        std::unique_ptr<Expression> value
    )
        :
        typeName(typeName),
        name(std::move(name)),
        value(std::move(value))
    {
    }



    std::string ToString() const override
    {
        return 
            "VariableDeclaration(" +
            typeName +
            " " +
            name->ToString() +
            " = " +
            value->ToString() +
            ")";
    }

};

}