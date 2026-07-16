#include "SemanticAnalyzer.hpp"

#include <iostream>
#include <stdexcept>
#include "../ast/BinaryExpression.hpp"

namespace nexus::semantic
{

void SemanticAnalyzer::Analyze(nexus::ast::Program& program)
{
    diagnostics.clear();
    symbols = SymbolTable();

    for (const auto& stmt : program.Statements())
    {
        AnalyzeStatement(stmt.get());
    }
}

void SemanticAnalyzer::Analyze(nexus::ast::Program* program)
{
    if (!program)
        return;

    Analyze(*program);
}

void SemanticAnalyzer::AnalyzeStatement(nexus::ast::Statement* stmt)
{
    if (!stmt)
        return;

    if (auto* decl = dynamic_cast<nexus::ast::VariableDeclaration*>(stmt))
    {
        AnalyzeVariableDeclaration(decl);
    }
    else if (auto* assign = dynamic_cast<nexus::ast::AssignmentStatement*>(stmt))
    {
        AnalyzeAssignmentStatement(assign);
    }
    else if (auto* exprStmt = dynamic_cast<nexus::ast::ExpressionStatement*>(stmt))
    {
        AnalyzeExpressionStatement(exprStmt);
    }
    else if (auto* ifStmt = dynamic_cast<nexus::ast::IfStatement*>(stmt))
    {
        AnalyzeIfStatement(ifStmt);
    }
    else if (auto* whileStmt = dynamic_cast<nexus::ast::WhileStatement*>(stmt))
    {
        AnalyzeWhileStatement(whileStmt);
    }
    else if (auto* fn = dynamic_cast<nexus::ast::FunctionDeclaration*>(stmt))
    {
        AnalyzeFunctionDeclaration(fn);
    }
    else if (auto* ret = dynamic_cast<nexus::ast::ReturnStatement*>(stmt))
    {
        AnalyzeReturnStatement(ret);
    }
    else if (dynamic_cast<nexus::ast::BreakStatement*>(stmt) || dynamic_cast<nexus::ast::ContinueStatement*>(stmt))
    {
        return;
    }
}

void SemanticAnalyzer::AnalyzeVariableDeclaration(nexus::ast::VariableDeclaration* stmt)
{
    if (!stmt || !stmt->Value())
        return;

    const std::string name = stmt->Name();
    if (symbols.Exists(name))
    {
        AddDiagnostic("Error: variable '" + name + "' already defined");
        return;
    }

    Type declared = Type::Unknown;
    if (stmt->TypeName() == "정수") declared = Type::Integer;
    else if (stmt->TypeName() == "실수") declared = Type::Float;
    else if (stmt->TypeName() == "불린") declared = Type::Boolean;
    else if (stmt->TypeName() == "문자열") declared = Type::String;

    Type inferred = AnalyzeExpression(stmt->Value());
    if (declared != Type::Unknown && inferred != Type::Unknown && declared != inferred)
    {
        if (!(declared == Type::Float && inferred == Type::Integer))
        {
            AddDiagnostic("Error: type mismatch in declaration of '" + name + "'");
        }
    }
    symbols.Define(name, declared != Type::Unknown ? declared : inferred);
}

void SemanticAnalyzer::AnalyzeAssignmentStatement(nexus::ast::AssignmentStatement* stmt)
{
    if (!stmt)
        return;

    const std::string name = stmt->TargetName();
    if (!symbols.Exists(name))
    {
        AddDiagnostic("Error: undefined variable '" + name + "'");
        return;
    }

    Type varType = symbols.Get(name);
    Type valType = AnalyzeExpression(stmt->Value());
    if (varType != Type::Unknown && valType != Type::Unknown && varType != valType)
    {
        if (!(varType == Type::Float && valType == Type::Integer))
        {
            AddDiagnostic("Error: type mismatch in assignment to '" + name + "'");
        }
    }
}

void SemanticAnalyzer::AnalyzeExpressionStatement(nexus::ast::ExpressionStatement* stmt)
{
    if (!stmt)
        return;

    AnalyzeExpression(stmt->GetExpression());
}

void SemanticAnalyzer::AnalyzeIfStatement(nexus::ast::IfStatement* stmt)
{
    if (!stmt)
        return;

    Type condType = AnalyzeExpression(stmt->Condition());
    if (condType != Type::Boolean && condType != Type::Unknown)
    {
        AddDiagnostic("Error: condition must be boolean");
    }
    AnalyzeBlock(stmt->Body());

    for (const auto& elif : stmt->Elifs())
    {
        Type elifCondType = AnalyzeExpression(elif.condition.get());
        if (elifCondType != Type::Boolean && elifCondType != Type::Unknown)
        {
            AddDiagnostic("Error: condition must be boolean");
        }
        AnalyzeBlock(elif.body.get());
    }

    if (stmt->ElseBody())
    {
        AnalyzeBlock(stmt->ElseBody());
    }
}

void SemanticAnalyzer::AnalyzeWhileStatement(nexus::ast::WhileStatement* stmt)
{
    if (!stmt)
        return;

    Type condType = AnalyzeExpression(stmt->Condition());
    if (condType != Type::Boolean && condType != Type::Unknown)
    {
        AddDiagnostic("Error: condition must be boolean");
    }
    AnalyzeBlock(stmt->Body());
}

void SemanticAnalyzer::AnalyzeFunctionDeclaration(nexus::ast::FunctionDeclaration* stmt)
{
    if (!stmt)
        return;

    symbols.Define(stmt->Name(), Type::Unknown);
    AnalyzeBlock(stmt->Body());
}

void SemanticAnalyzer::AnalyzeReturnStatement(nexus::ast::ReturnStatement* stmt)
{
    if (!stmt)
        return;

    if (stmt->Value())
    {
        AnalyzeExpression(stmt->Value());
    }
}

void SemanticAnalyzer::AnalyzeBlock(nexus::ast::BlockStatement* block)
{
    if (!block)
        return;

    for (const auto& stmt : block->Statements())
    {
        AnalyzeStatement(stmt.get());
    }
}

Type SemanticAnalyzer::AnalyzeExpression(nexus::ast::Expression* expr)
{
    if (!expr)
        return Type::Unknown;

    if (auto* id = dynamic_cast<nexus::ast::Identifier*>(expr))
        return AnalyzeIdentifier(id);

    if (dynamic_cast<nexus::ast::IntegerLiteral*>(expr))
        return Type::Integer;

    if (dynamic_cast<nexus::ast::FloatLiteral*>(expr))
        return Type::Float;

    if (auto* call = dynamic_cast<nexus::ast::CallExpression*>(expr))
        return AnalyzeCallExpression(call);

    if (auto* binary = dynamic_cast<nexus::ast::BinaryExpression*>(expr))
    {
        Type leftType = AnalyzeExpression(binary->Left());
        Type rightType = AnalyzeExpression(binary->Right());
        std::string op = binary->Operator();
        if (op == "<" || op == ">" || op == "<=" || op == ">=" || op == "==" || op == "!=")
        {
            return Type::Boolean;
        }
        if (leftType == Type::Float || rightType == Type::Float)
        {
            return Type::Float;
        }
        return leftType;
    }

    return Type::Unknown;
}

Type SemanticAnalyzer::AnalyzeIdentifier(nexus::ast::Identifier* expr)
{
    if (!expr)
        return Type::Unknown;

    const std::string name = expr->Name();
    if (!symbols.Exists(name))
    {
        AddDiagnostic("Error: undefined variable '" + name + "'");
        return Type::Unknown;
    }

    return symbols.Get(name);
}

Type SemanticAnalyzer::AnalyzeCallExpression(nexus::ast::CallExpression* expr)
{
    if (!expr)
        return Type::Unknown;

    return Type::Unknown;
}

Type SemanticAnalyzer::AnalyzeLiteral(nexus::ast::Expression* expr)
{
    (void)expr;
    return Type::Unknown;
}

void SemanticAnalyzer::AddDiagnostic(const std::string& message)
{
    diagnostics.push_back(message);
}

}