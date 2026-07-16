#include "Parser.hpp"

#include "../ast/VariableDeclaration.hpp"
#include "../ast/IntegerLiteral.hpp"
#include "../ast/Expression.hpp"
#include "../ast/Statement.hpp"
#include "../ast/ExpressionStatement.hpp"
#include "../ast/Identifier.hpp"
#include "../ast/CallExpression.hpp"
#include "../ast/BinaryExpression.hpp"
#include "../ast/FloatLiteral.hpp"
#include "../ast/IfStatement.hpp"
#include "../ast/WhileStatement.hpp"
#include "../ast/AssignmentStatement.hpp"
#include "../ast/BreakStatement.hpp"
#include "../ast/ContinueStatement.hpp"
#include "../ast/FunctionDeclaration.hpp"
#include "../ast/ReturnStatement.hpp"

namespace nexus::parser
{


Parser::Parser(
    nexus::token::TokenStream stream
)
    :
    stream(std::move(stream))
{
}



std::unique_ptr<nexus::ast::Program>
Parser::Parse()
{
    auto program =
        std::make_unique<nexus::ast::Program>();


    while(!stream.IsEnd())
    {
        program->AddStatement(
            ParseStatement()
        );
    }


    return program;
}



std::unique_ptr<nexus::ast::Statement>
Parser::ParseStatement()
{
    switch(stream.Current().type)
    {
    case nexus::token::TokenType::TypeFloat:
    case nexus::token::TokenType::TypeString:
    case nexus::token::TokenType::TypeBoolean:
    case nexus::token::TokenType::TypeInteger:
        return ParseVariableDeclaration();

    case nexus::token::TokenType::Print:
    {
        auto expr = ParseCallExpression();

        return std::make_unique<nexus::ast::ExpressionStatement>(
            std::move(expr)
        );
    }

    case nexus::token::TokenType::If:
        return ParseIfStatement();

    case nexus::token::TokenType::While:
        return ParseWhileStatement();

    case nexus::token::TokenType::Identifier:
        return ParseAssignmentStatement();

    case nexus::token::TokenType::Break:
        return ParseBreakStatement();

    case nexus::token::TokenType::Continue:
        return ParseContinueStatement();

    case nexus::token::TokenType::Function:
        return ParseFunctionDeclaration();

    case nexus::token::TokenType::Return:
        return ParseReturnStatement();

    default:
        stream.Consume();
        return nullptr;
    }
}



std::unique_ptr<nexus::ast::Statement>
Parser::ParseVariableDeclaration()
{
    auto type =
        stream.Consume();


    auto name =
        stream.Consume();


    stream.Consume(); // =


    auto value =
        ParseExpression();


    return std::make_unique<nexus::ast::VariableDeclaration>(
        type.value,
        std::make_unique<nexus::ast::Identifier>(
            name.value
        ),
        std::move(value)
    );
}



std::unique_ptr<nexus::ast::Expression>
Parser::ParseCallExpression()
{
    auto function =
        stream.Consume();


    auto call =
        std::make_unique<nexus::ast::CallExpression>(
            function.value
        );


    // (
    if(stream.Check(nexus::token::TokenType::LeftParen))
    {
        stream.Consume();
    }


    // 인자 읽기
    while(!stream.Check(nexus::token::TokenType::RightParen) && !stream.IsEnd())
    {
        call->AddArgument(
            ParseExpression()
        );

        if(stream.Check(nexus::token::TokenType::Comma))
        {
            stream.Consume();
        }
        else
        {
            break;
        }
    }


    // )
    if(stream.Check(nexus::token::TokenType::RightParen))
    {
        stream.Consume();
    }


    return call;
}

std::unique_ptr<nexus::ast::Expression>
Parser::ParseExpression()
{
    return ParseBinaryExpression(0);
}

std::unique_ptr<nexus::ast::Expression>
Parser::ParsePrimary()
{
    auto token =
        stream.Consume();


    using namespace nexus::token;


    switch(token.type)
    {

    case TokenType::IntegerLiteral:

        return std::make_unique<
            nexus::ast::IntegerLiteral
        >(
            std::stoll(token.value)
        );


    case TokenType::FloatLiteral:

        return std::make_unique<
            nexus::ast::FloatLiteral
        >(
            std::stof(token.value)
        );


    case TokenType::Identifier:

        return std::make_unique<
            nexus::ast::Identifier
        >(
            token.value
        );


    case TokenType::LeftParen:
    {
        auto expr =
            ParseExpression();

        if(stream.Check(
            TokenType::RightParen))
        {
            stream.Consume();
        }
        
        
        return expr;
    }
    
    default:

        return nullptr;
    }
}

std::unique_ptr<nexus::ast::Expression>
Parser::ParseBinaryExpression(
    int precedence
)
{
    auto left = ParsePrimary();


    while(true)
    {
        auto op = stream.Current();


        int opPrecedence =
            GetPrecedence(op.type);


        if(opPrecedence <= precedence)
            break;


        stream.Consume();


        auto right =
            ParseBinaryExpression(
                opPrecedence
            );


        left =
            std::make_unique<
                nexus::ast::BinaryExpression
            >(
                op.value,
                std::move(left),
                std::move(right)
            );
    }


    return left;
}

int Parser::GetPrecedence(nexus::token::TokenType type)
{
    switch(type)
    {

    case nexus::token::TokenType::Multiply:
    case nexus::token::TokenType::Divide:
        return 30;


    case nexus::token::TokenType::Plus:
    case nexus::token::TokenType::Minus:
        return 20;


    case nexus::token::TokenType::Greater:
    case nexus::token::TokenType::Less:
    case nexus::token::TokenType::GreaterEqual:
    case nexus::token::TokenType::LessEqual:
        return 10;


    case nexus::token::TokenType::Equal:
    case nexus::token::TokenType::NotEqual:
        return 5;


    default:
        return 0;
    }
}

std::unique_ptr<nexus::ast::BlockStatement>
Parser::ParseBlock()
{
    auto block =
        std::make_unique<nexus::ast::BlockStatement>();


    // {

    if(stream.Check(nexus::token::TokenType::LeftBrace))
    {
        stream.Consume();
    }


    while(
        !stream.Check(nexus::token::TokenType::RightBrace) &&
        !stream.IsEnd()
    )
    {
        auto stmt =
            ParseStatement();


        if(stmt)
            block->AddStatement(
                std::move(stmt)
            );
    }


    // }

    if(stream.Check(nexus::token::TokenType::RightBrace))
    {
        stream.Consume();
    }


    return block;
}

std::unique_ptr<nexus::ast::Statement>
Parser::ParseAssignmentStatement()
{
    auto target = std::make_unique<nexus::ast::Identifier>(stream.Consume().value);

    stream.Consume();

    auto value = ParseExpression();

    return std::make_unique<nexus::ast::AssignmentStatement>(
        std::move(target),
        std::move(value)
    );
}

std::unique_ptr<nexus::ast::Statement>
Parser::ParseBreakStatement()
{
    stream.Consume();
    return std::make_unique<nexus::ast::BreakStatement>();
}

std::unique_ptr<nexus::ast::Statement>
Parser::ParseContinueStatement()
{
    stream.Consume();
    return std::make_unique<nexus::ast::ContinueStatement>();
}

std::unique_ptr<nexus::ast::Statement>
Parser::ParseReturnStatement()
{
    stream.Consume();

    std::unique_ptr<nexus::ast::Expression> value = nullptr;

    if(!stream.IsEnd() && !stream.Check(nexus::token::TokenType::RightBrace))
    {
        value = ParseExpression();
    }

    return std::make_unique<nexus::ast::ReturnStatement>(std::move(value));
}

std::unique_ptr<nexus::ast::Statement>
Parser::ParseFunctionDeclaration()
{
    stream.Consume();

    auto nameToken = stream.Consume();
    auto name = nameToken.value;

    std::vector<nexus::ast::Parameter> parameters;

    if(stream.Check(nexus::token::TokenType::LeftParen))
    {
        stream.Consume();

        while(!stream.Check(nexus::token::TokenType::RightParen) && !stream.IsEnd())
        {
            auto paramName = stream.Consume();
            stream.Consume();
            auto paramType = stream.Consume();

            parameters.push_back({paramName.value, paramType.value});

            if(stream.Check(nexus::token::TokenType::Comma))
            {
                stream.Consume();
            }
        }

        if(stream.Check(nexus::token::TokenType::RightParen))
        {
            stream.Consume();
        }
    }

    std::string returnType = "Void";

    if(stream.Check(nexus::token::TokenType::Colon))
    {
        stream.Consume();
        auto returnTypeToken = stream.Consume();
        returnType = returnTypeToken.value;
    }

    auto body = ParseBlock();

    return std::make_unique<nexus::ast::FunctionDeclaration>(
        std::move(name),
        std::move(parameters),
        std::move(returnType),
        std::move(body)
    );
}

std::unique_ptr<nexus::ast::Statement>
Parser::ParseIfStatement()
{
    stream.Consume();

    auto condition =
        ParseExpression();

    auto body =
        ParseBlock();

    auto statement =
        std::make_unique<nexus::ast::IfStatement>(
            std::move(condition),
            std::move(body)
        );

    while(stream.Check(nexus::token::TokenType::Elif))
    {
        stream.Consume();

        auto elifCondition =
            ParseExpression();

        auto elifBody =
            ParseBlock();

        statement->AddElseIf(
            std::move(elifCondition),
            std::move(elifBody)
        );
    }

    if(stream.Check(nexus::token::TokenType::Else))
    {
        stream.Consume();

        auto elseBody =
            ParseBlock();

        statement->SetElse(
            std::move(elseBody)
        );
    }

    return statement;
}

std::unique_ptr<nexus::ast::Statement>
Parser::ParseWhileStatement()
{
    stream.Consume();

    auto condition =
        ParseExpression();

    auto body =
        ParseBlock();

    return std::make_unique<nexus::ast::WhileStatement>(
        std::move(condition),
        std::move(body)
    );
}

}