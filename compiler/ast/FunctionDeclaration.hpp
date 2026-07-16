#pragma once

#include "Statement.hpp"
#include "Expression.hpp"
#include "BlockStatement.hpp"
#include "Identifier.hpp"

#include <memory>
#include <string>
#include <vector>

namespace nexus::ast
{

struct Parameter
{
    std::string name;
    std::string typeName;
};

class FunctionDeclaration : public Statement
{
private:

    std::string name;
    std::vector<Parameter> parameters;
    std::string returnType;
    std::unique_ptr<BlockStatement> body;

public:

    FunctionDeclaration(
        std::string name,
        std::vector<Parameter> parameters,
        std::string returnType,
        std::unique_ptr<BlockStatement> body
    )
        : name(std::move(name)),
          parameters(std::move(parameters)),
          returnType(std::move(returnType)),
          body(std::move(body))
    {
    }

    const std::string& Name() const
    {
        return name;
    }

    const std::vector<Parameter>& Parameters() const
    {
        return parameters;
    }

    const std::string& ReturnType() const
    {
        return returnType;
    }

    BlockStatement* Body() const
    {
        return body.get();
    }

    std::string ToString() const override
    {
        return "FunctionDeclaration";
    }
};

}