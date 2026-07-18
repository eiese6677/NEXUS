#include "SemanticAnalyzer.hpp"

#include <iostream>
#include <stdexcept>
#include "../ast/BinaryExpression.hpp"
#include "../ast/StringLiteral.hpp"
#include "../ast/BooleanLiteral.hpp"

namespace nexus::semantic
{

static Type ConvertType(const std::string& name)
{
    if (name == "정수" || name == "int") return Type::Int;
    if (name == "실수" || name == "float") return Type::Float;
    if (name == "문자열" || name == "String") return Type::String;
    if (name == "불린" || name == "bool") return Type::Bool;

    return Type::Unknown;
}

void SemanticAnalyzer::Analyze(nexus::ast::Program& program)
{
    diagnostics.clear();
    symbols.Clear();
    symbols.PushScope();   // Global

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
    if (stmt->TypeName() == "정수") declared = Type::Int;
    else if (stmt->TypeName() == "실수") declared = Type::Float;
    else if (stmt->TypeName() == "불린") declared = Type::Bool;
    else if (stmt->TypeName() == "문자열") declared = Type::String;

    Type inferred = AnalyzeExpression(stmt->Value());
    if (declared != Type::Unknown && inferred != Type::Unknown && declared != inferred)
    {
        if (!(declared == Type::Float && inferred == Type::Int))
        {
            AddDiagnostic("Error: type mismatch in declaration of '" + name + "'");
        }
    }
    Type finalType =
        declared != Type::Unknown
            ? declared
            : inferred;

    symbols.Define(
        name,
        finalType,
        stmt->IsConst()
    );
}

void SemanticAnalyzer::AnalyzeAssignmentStatement(nexus::ast::AssignmentStatement* stmt)
{
    
    if (!stmt)
    return;
    
    const std::string name = stmt->TargetName();
    
    if (!symbols.Exists(name))
    {
        AddDiagnostic(
            "Error: undefined variable '" + name + "'"
        );
        return;
    }

    Symbol symbol = symbols.Get(name);

    if (symbol.isConst)
    {
        AddDiagnostic(
            "Error: cannot assign to const variable '" + name + "'"
        );
        return;
    }

    Type varType = symbol.type;
    Type valType = AnalyzeExpression(stmt->Value());

    if (varType != Type::Unknown &&
        valType != Type::Unknown &&
        varType != valType)
    {
        if (!(varType == Type::Float &&
            valType == Type::Int))
        {
            AddDiagnostic(
                "Error: type mismatch in assignment to '" + name + "'"
            );
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
    if (condType != Type::Bool && condType != Type::Unknown)
    {
        AddDiagnostic("Error: condition must be boolean");
    }
    AnalyzeBlock(stmt->Body());

    for (const auto& elif : stmt->Elifs())
    {
        Type elifCondType = AnalyzeExpression(elif.condition.get());
        if (elifCondType != Type::Bool && elifCondType != Type::Unknown)
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
    if (condType != Type::Bool && condType != Type::Unknown)
    {
        AddDiagnostic("Error: condition must be boolean");
    }
    AnalyzeBlock(stmt->Body());
}

void SemanticAnalyzer::AnalyzeFunctionDeclaration(nexus::ast::FunctionDeclaration* stmt)
{
    if (!stmt)
        return;
        
    symbols.PushScope();

    for (const auto& param : stmt->Parameters())
    {
        symbols.Define(
            param.name,
            ConvertType(param.typeName)
        );
    }

    AnalyzeBlock(stmt->Body());

    symbols.PopScope();
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

    symbols.PushScope();
    
    for (const auto& stmt : block->Statements())
    {
        AnalyzeStatement(stmt.get());
    }

    symbols.PopScope();
}

Type SemanticAnalyzer::AnalyzeExpression(nexus::ast::Expression* expr)
{
    if (!expr)
        return Type::Unknown;

    if (auto* id = dynamic_cast<nexus::ast::Identifier*>(expr))
        return AnalyzeIdentifier(id).type;

    if (dynamic_cast<nexus::ast::IntegerLiteral*>(expr))
        return Type::Int;

    if (dynamic_cast<nexus::ast::FloatLiteral*>(expr))
        return Type::Float;

    if (dynamic_cast<nexus::ast::StringLiteral*>(expr))
        return Type::String;

    if(dynamic_cast<nexus::ast::BooleanLiteral*>(expr))
        return Type::Bool;

    if (auto* call = dynamic_cast<nexus::ast::CallExpression*>(expr))
        return AnalyzeCallExpression(call);

    if (auto* binary =
    dynamic_cast<nexus::ast::BinaryExpression*>(expr))
    {
        Type leftType =
            AnalyzeExpression(binary->Left());

        Type rightType =
            AnalyzeExpression(binary->Right());

        const std::string op =
            binary->Operator();

        // 비교 연산
        if (op == "<" ||
            op == ">" ||
            op == "<=" ||
            op == ">=" ||
            op == "==" ||
            op == "!=")
        {
            if (leftType == rightType ||
                ((leftType == Type::Int || leftType == Type::Float) &&
                (rightType == Type::Int || rightType == Type::Float)))
            {
                return Type::Bool;
            }

            AddDiagnostic(
                "Error: invalid comparison"
            );
            return Type::Unknown;
        }

        // 문자열 연결
        if (op == "+")
        {
            if (leftType == Type::String &&
                rightType == Type::String)
            {
                return Type::String;
            }
        }

        // 숫자 연산
        if ((leftType == Type::Int ||
            leftType == Type::Float) &&
            (rightType == Type::Int ||
            rightType == Type::Float))
        {
            if (leftType == Type::Float ||
                rightType == Type::Float)
            {
                return Type::Float;
            }

            return Type::Int;
        }

        AddDiagnostic(
            "Error: invalid operands for operator '" + op + "'"
        );

        return Type::Unknown;
    }

    return Type::Unknown;
}

Symbol SemanticAnalyzer::AnalyzeIdentifier(
    nexus::ast::Identifier* expr
)
{
    if (!expr)
    {
        return {};
    }

    const std::string& name = expr->Name();

    if (!symbols.Exists(name))
    {
        AddDiagnostic(
            "Error: undefined variable '" + name + "'"
        );

        return {};
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

Type SemanticAnalyzer::InferExpressionType(
    ast::Expression* expr
)
{
    if(auto* integer =
        dynamic_cast<ast::IntegerLiteral*>(expr))
    {
        return Type::Int;
    }


    if(auto* floating =
        dynamic_cast<ast::FloatLiteral*>(expr))
    {
        return Type::Float;
    }


    if(auto* str =
        dynamic_cast<ast::StringLiteral*>(expr))
    {
        return Type::String;
    }


    if(auto* binary =
        dynamic_cast<ast::BinaryExpression*>(expr))
    {
        auto left =
            InferExpressionType(binary->Left());

        auto right =
            InferExpressionType(binary->Right());


        if(left == Type::Float ||
           right == Type::Float)
        {
            return Type::Float;
        }


        if(left == Type::Int &&
           right == Type::Int)
        {
            return Type::Int;
        }
    }


    return Type::Unknown;
}

}