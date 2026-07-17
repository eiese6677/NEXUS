#include "IRBuilder.hpp"

#include "../ast/VariableDeclaration.hpp"
#include "../ast/BinaryExpression.hpp"
#include "../ast/IntegerLiteral.hpp"
#include "../ast/FloatLiteral.hpp"
#include "../ast/Identifier.hpp"
#include "../ast/CallExpression.hpp"
#include "../ast/ExpressionStatement.hpp"
#include "../ast/IfStatement.hpp"
#include "../ast/WhileStatement.hpp"
#include "../ast/AssignmentStatement.hpp"
#include "../ast/BlockStatement.hpp"
#include "../ast/BreakStatement.hpp"
#include "../ast/ContinueStatement.hpp"
#include "../ast/StringLiteral.hpp"
#include "../ast/BooleanLiteral.hpp"
#include "../ast/FunctionDeclaration.hpp"
#include "../ast/ReturnStatement.hpp"

#include <iostream>
#include <string>

namespace nexus::ir
{

IRModule IRBuilder::Build(nexus::ast::Program& program)
{
    module = IRModule();
    labelCounter = 0;
    loopStack.clear();

    // 1. 함수 생성
    for (const auto& statement : program.Statements())
    {
        if (dynamic_cast<nexus::ast::FunctionDeclaration*>(statement.get()))
        {
            GenerateStatement(statement.get());
        }
    }


    // 2. MAIN 시작
    module.Add({
        Opcode::Label,
        "MAIN"
    });


    // 3. 일반 실행 코드 생성
    for (const auto& statement : program.Statements())
    {
        if (!dynamic_cast<nexus::ast::FunctionDeclaration*>(statement.get()))
        {
            GenerateStatement(statement.get());
        }
    }


    return std::move(module);
}

void IRBuilder::GenerateStatement(nexus::ast::Statement* statement)
{
    if (!statement)
        return;

    if (auto* var = dynamic_cast<nexus::ast::VariableDeclaration*>(statement))
    {
        GenerateExpression(var->Value());
        module.Add({nexus::ir::Opcode::Store, var->Name()});
        return;
    }

    if (auto* assign = dynamic_cast<nexus::ast::AssignmentStatement*>(statement))
    {
        GenerateExpression(assign->Value());
        module.Add({nexus::ir::Opcode::Store, assign->TargetName()});
        return;
    }

    if (auto* fn = dynamic_cast<nexus::ast::FunctionDeclaration*>(statement))
    {
        GenerateFunction(fn);
        return;
    }

    if (auto* ret =
        dynamic_cast<nexus::ast::ReturnStatement*>(statement))
    {
        if (ret->Value())
        {
            GenerateExpression(ret->Value());
        }

        module.Add({
            nexus::ir::Opcode::Return,
            std::monostate{}
        });

        return;
    }

    if (auto* exprStmt = dynamic_cast<nexus::ast::ExpressionStatement*>(statement))
    {
        GenerateExpression(exprStmt->GetExpression());
        return;
    }

    if (auto* ifStmt = dynamic_cast<nexus::ast::IfStatement*>(statement))
    {
        std::string falseLabel = NextLabel();
        std::string endLabel = "";
        if (!ifStmt->Elifs().empty() || ifStmt->ElseBody())
        {
            endLabel = NextLabel();
        }

        GenerateExpression(ifStmt->Condition());
        module.Add({nexus::ir::Opcode::JumpIfFalse, falseLabel});
        GenerateBlock(ifStmt->Body());
        if (!endLabel.empty())
        {
            module.Add({nexus::ir::Opcode::Jump, endLabel});
        }

        for (const auto& elif : ifStmt->Elifs())
        {
            module.Add({nexus::ir::Opcode::Label, falseLabel});
            falseLabel = NextLabel();
            GenerateExpression(elif.condition.get());
            module.Add({nexus::ir::Opcode::JumpIfFalse, falseLabel});
            GenerateBlock(elif.body.get());
            module.Add({nexus::ir::Opcode::Jump, endLabel});
        }

        if (ifStmt->ElseBody())
        {
            module.Add({nexus::ir::Opcode::Label, falseLabel});
            GenerateBlock(ifStmt->ElseBody());
        }
        else
        {
            module.Add({nexus::ir::Opcode::Label, falseLabel});
        }

        if (!endLabel.empty())
        {
            module.Add({nexus::ir::Opcode::Label, endLabel});
        }
        return;
    }

    if (auto* whileStmt = dynamic_cast<nexus::ast::WhileStatement*>(statement))
    {
        std::string startLabel = NextLabel();
        std::string endLabel = NextLabel();

        loopStack.push_back({startLabel, endLabel});

        module.Add({nexus::ir::Opcode::Label, startLabel});
        GenerateExpression(whileStmt->Condition());
        module.Add({nexus::ir::Opcode::JumpIfFalse, endLabel});
        GenerateBlock(whileStmt->Body());
        module.Add({nexus::ir::Opcode::Jump, startLabel});
        module.Add({nexus::ir::Opcode::Label, endLabel});

        loopStack.pop_back();
        return;
    }

    if (dynamic_cast<nexus::ast::BreakStatement*>(statement))
    {
        if (!loopStack.empty())
        {
            module.Add({nexus::ir::Opcode::Jump, loopStack.back().second});
        }
        return;
    }

    if (dynamic_cast<nexus::ast::ContinueStatement*>(statement))
    {
        if (!loopStack.empty())
        {
            module.Add({nexus::ir::Opcode::Jump, loopStack.back().first});
        }
        return;
    }
}

void IRBuilder::GenerateBlock(nexus::ast::BlockStatement* block)
{
    if (!block)
        return;
    for (const auto& stmt : block->Statements())
    {
        GenerateStatement(stmt.get());
    }
}

std::string IRBuilder::NextLabel()
{
    return "L" + std::to_string(labelCounter++);
}

void IRBuilder::GenerateExpression(nexus::ast::Expression* expression)
{
    if (!expression)
        return;
    
    if (auto* str =
        dynamic_cast<nexus::ast::StringLiteral*>(expression))
    {
        module.Add({
            nexus::ir::Opcode::LoadConstant,
            str->Value()
        });
        return;
    }

    if (auto* integer = dynamic_cast<nexus::ast::IntegerLiteral*>(expression))
    {
        module.Add({
            Opcode::LoadConstant,
            int64_t(integer->Value())
        });
        return;
    }

    if (auto* boolean =
        dynamic_cast<nexus::ast::BooleanLiteral*>(expression))
    {
        module.Add({
            nexus::ir::Opcode::LoadConstant,
            boolean->Value()
        });

        return;
    }

    if (auto* floatLit = dynamic_cast<nexus::ast::FloatLiteral*>(expression))
    {
        module.Add({
            Opcode::LoadConstant,
            floatLit->Value()
        });
        return;
    }

    if (auto* call = dynamic_cast<nexus::ast::CallExpression*>(expression))
    {
        for (const auto& arg : call->Arguments())
        {
            GenerateExpression(arg.get());
        }

        module.Add({
            nexus::ir::Opcode::Call,
            call->Name()
        });

        return;
    }

    if (auto* identifier = dynamic_cast<nexus::ast::Identifier*>(expression))
    {
        module.Add({nexus::ir::Opcode::LoadVariable, identifier->Name()});
        return;
    }

    if (auto* binary = dynamic_cast<nexus::ast::BinaryExpression*>(expression))
    {
        GenerateExpression(binary->Left());
        GenerateExpression(binary->Right());

        std::string op = binary->Operator();
        if (op == "+")
        {
            module.Add({
                nexus::ir::Opcode::Add,
                std::monostate{}
            });
        }
        else if (op == "-")
        {
            module.Add({
                nexus::ir::Opcode::Subtract,
                std::monostate{}
            });
        }
        else if (op == "*")
        {
            module.Add({
                nexus::ir::Opcode::Multiply,
                std::monostate{}
            });
        }
        else if (op == "/")
        {
            module.Add({
                nexus::ir::Opcode::Divide,
                std::monostate{}
            });
        }
        else if (op == "<" || op == ">" || op == "<=" || op == ">=" || op == "==" || op == "!=")
        {
            module.Add({nexus::ir::Opcode::Compare, op});
        }

        return;
    }
}

// void IRBuilder::GenerateFunction(
//     nexus::ast::FunctionDeclaration* fn
// )
// {
//     if(fn->Name() == "MAIN")
//         return;


//     size_t address =
//         module.Instructions().size();

//     std::vector<std::string> parameters;

//     for(const auto& param : fn->Parameters())
//     {
//         parameters.push_back(param.name);
//     }

//     module.AddFunction(
//         fn->Name(),
//         address,
//         parameters
//     );

//     module.Add({
//         Opcode::Function,
//         fn->Name()
//     });


//     GenerateBlock(fn->Body());
// }
void IRBuilder::GenerateFunction(
    nexus::ast::FunctionDeclaration* fn
)
{
    std::vector<std::string> parameters;

    for(const auto& param : fn->Parameters())
    {
        parameters.push_back(param.name);
    }


    module.AddFunction(
        fn->Name(),
        0,
        parameters
    );


    module.Add({
        Opcode::Function,
        fn->Name()
    });


    inFunction = true;

    GenerateBlock(fn->Body());

    inFunction = false;
}

}
