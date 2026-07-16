#pragma once

#include "Statement.hpp"
#include "Expression.hpp"
#include "Identifier.hpp"

#include <memory>

namespace nexus::ast
{

class AssignmentStatement : public Statement
{
private:

    std::unique_ptr<Identifier> target;
    std::unique_ptr<Expression> value;

public:

    AssignmentStatement(
        std::unique_ptr<Identifier> target,
        std::unique_ptr<Expression> value
    )
        : target(std::move(target)),
          value(std::move(value))
    {
    }

    std::string ToString() const override
    {
        return "AssignmentStatement";
    }
};

}