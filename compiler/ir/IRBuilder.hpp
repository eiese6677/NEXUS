#pragma once

#include "IRModule.hpp"
#include <vector>
#include <utility>

#include "../ast/Program.hpp"
#include "../ast/Statement.hpp"
#include "../ast/Expression.hpp"
#include "../ast/BlockStatement.hpp"

namespace nexus::ir
{

class IRBuilder
{
private:
    IRModule module;
    int labelCounter;
    std::vector<std::pair<std::string, std::string>> loopStack;

public:
    IRModule Build(nexus::ast::Program& program);

private:
    void GenerateStatement(nexus::ast::Statement* statement);
    void GenerateExpression(nexus::ast::Expression* expression);
    void GenerateBlock(nexus::ast::BlockStatement* block);
    std::string NextLabel();
};

}
