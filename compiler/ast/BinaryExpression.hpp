#pragma once

#include "Expression.hpp"

#include <memory>
#include <string>


namespace nexus::ast
{

class BinaryExpression : public Expression
{
private:

    std::string op;

    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;


public:

    BinaryExpression(
        const std::string& op,
        std::unique_ptr<Expression> left,
        std::unique_ptr<Expression> right
    )
        :
        op(op),
        left(std::move(left)),
        right(std::move(right))
    {
    }


    std::string ToString() const override
    {
        return
            "Binary(" +
            left->ToString() +
            " " +
            op +
            " " +
            right->ToString() +
            ")";
    }

};

}