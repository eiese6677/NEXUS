#include <iostream>
#include "ast/literal.hpp"

int main()
{
    nexus::ast::NumberLiteral node(123);

    std::cout << node.ToString();

    return 0;
}