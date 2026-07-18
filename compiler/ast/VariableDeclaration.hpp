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
    bool isConst;

    std::string typeName;

    std::unique_ptr<Identifier> name;

    std::unique_ptr<Expression> value;

public:
    VariableDeclaration(
        bool isConst,
        const std::string& typeName,
        std::unique_ptr<Identifier> name,
        std::unique_ptr<Expression> value
    )
        :
        isConst(isConst),
        typeName(typeName),
        name(std::move(name)),
        value(std::move(value))
    {
    }

    bool IsConst() const
    {
        return isConst;
    }

    const std::string& TypeName() const
    {
        return typeName;
    }

    const std::string& Name() const
    {
        return name ? name->Name() : typeName;
    }

    Expression* Value() const
    {
        return value.get();
    }

    std::string ToString() const override
    {
        return
            std::string(isConst ? "ConstDeclaration(" : "VariableDeclaration(") +
            typeName +
            " " +
            name->ToString() +
            " = " +
            value->ToString() +
            ")";
    }
};

}