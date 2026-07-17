#pragma once

#include "../token/TokenStream.hpp"
#include "../ast/Program.hpp"
#include "../ast/Expression.hpp"
#include "../ast/CallExpression.hpp"
#include "../ast/BlockStatement.hpp"
#include "../ast/WhileStatement.hpp"
#include "../ast/AssignmentStatement.hpp"
#include "../ast/BreakStatement.hpp"
#include "../ast/ContinueStatement.hpp"
#include "../ast/FunctionDeclaration.hpp"
#include "../ast/ReturnStatement.hpp"

#include <memory>


namespace nexus::parser
{

class Parser
{

private:

    nexus::token::TokenStream stream;

public:

    Parser(
        nexus::token::TokenStream stream
    );

    std::unique_ptr<nexus::ast::Program> Parse();

private:

    std::unique_ptr<nexus::ast::Statement>
    ParseStatement();

    std::unique_ptr<nexus::ast::Statement>
    ParseVariableDeclaration();

    std::unique_ptr<nexus::ast::Expression>
    ParseExpression();

    std::unique_ptr<nexus::ast::Expression>
    ParseCallExpression(std::string name);

    std::unique_ptr<nexus::ast::Expression>
    ParsePrimary();

    std::unique_ptr<nexus::ast::Expression>
    ParseBinaryExpression(
        int precedence
    );

    int GetPrecedence(
        nexus::token::TokenType type
    );

    std::unique_ptr<nexus::ast::Statement>
    ParseIfStatement();

    std::unique_ptr<nexus::ast::BlockStatement>
    ParseBlock();

    std::unique_ptr<nexus::ast::Statement>
    ParseWhileStatement();

    std::unique_ptr<nexus::ast::Statement>
    ParseAssignmentStatement();

    std::unique_ptr<nexus::ast::Statement>
    ParseBreakStatement();

    std::unique_ptr<nexus::ast::Statement>
    ParseContinueStatement();

    std::unique_ptr<nexus::ast::Statement>
    ParseFunctionDeclaration();

    std::unique_ptr<nexus::ast::Statement>
    ParseReturnStatement();

};

}