#pragma once

#include "SymbolTable.hpp"
#include "../ast/Program.hpp"
#include "../ast/Statement.hpp"
#include "../ast/VariableDeclaration.hpp"
#include "../ast/AssignmentStatement.hpp"
#include "../ast/ExpressionStatement.hpp"
#include "../ast/Identifier.hpp"
#include "../ast/CallExpression.hpp"
#include "../ast/IntegerLiteral.hpp"
#include "../ast/FloatLiteral.hpp"
#include "../ast/IfStatement.hpp"
#include "../ast/WhileStatement.hpp"
#include "../ast/FunctionDeclaration.hpp"
#include "../ast/ReturnStatement.hpp"
#include "../ast/BreakStatement.hpp"
#include "../ast/ContinueStatement.hpp"

#include <string>
#include <vector>

namespace nexus::semantic
{

class SemanticAnalyzer
{
private:
    SymbolTable symbols;
    std::vector<std::string> diagnostics;

public:
    void Analyze(nexus::ast::Program& program);
    void Analyze(nexus::ast::Program* program);
    const std::vector<std::string>& Diagnostics() const
    {
        return diagnostics;
    }

private:
    void AnalyzeStatement(nexus::ast::Statement* stmt);
    void AnalyzeVariableDeclaration(nexus::ast::VariableDeclaration* stmt);
    void AnalyzeAssignmentStatement(nexus::ast::AssignmentStatement* stmt);
    void AnalyzeExpressionStatement(nexus::ast::ExpressionStatement* stmt);
    void AnalyzeIfStatement(nexus::ast::IfStatement* stmt);
    void AnalyzeWhileStatement(nexus::ast::WhileStatement* stmt);
    void AnalyzeFunctionDeclaration(nexus::ast::FunctionDeclaration* stmt);
    void AnalyzeReturnStatement(nexus::ast::ReturnStatement* stmt);
    void AnalyzeBlock(nexus::ast::BlockStatement* block);

    Type AnalyzeExpression(nexus::ast::Expression* expr);
    Type AnalyzeIdentifier(nexus::ast::Identifier* expr);
    Type AnalyzeCallExpression(nexus::ast::CallExpression* expr);
    Type AnalyzeLiteral(nexus::ast::Expression* expr);
    void AddDiagnostic(const std::string& message);
};

}