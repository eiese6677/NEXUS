#pragma once

#include "TokenType.hpp"
#include <string>

namespace nexus::token
{

struct Token
{
    TokenType type = TokenType::Unknown;
    std::string value;

    int line = 0;
    int column = 0;
};

inline std::string TokenTypeToString(TokenType type)
{
    switch(type)
    {
    case TokenType::Identifier:
        return "Identifier";

    case TokenType::IntegerLiteral:
        return "IntegerLiteral";

    case TokenType::FloatLiteral:
        return "FloatLiteral";

    case TokenType::StringLiteral:
        return "StringLiteral";

    case TokenType::BooleanLiteral:
        return "BooleanLiteral";

    case TokenType::Let:
        return "Let";

    case TokenType::Const:
        return "Const";

    case TokenType::Print:
        return "Print";

    case TokenType::If:
        return "If";

    case TokenType::Else:
        return "Else";

    case TokenType::While:
        return "While";

    case TokenType::Function:
        return "Function";

    case TokenType::Return:
        return "Return";

    case TokenType::LeftParen:
        return "LeftParen";

    case TokenType::RightParen:
        return "RightParen";

    case TokenType::LeftBrace:
        return "LeftBrace";

    case TokenType::RightBrace:
        return "RightBrace";

    case TokenType::Plus:
        return "Plus";

    case TokenType::Minus:
        return "Minus";

    case TokenType::Multiply:
        return "Multiply";

    case TokenType::Divide:
        return "Divide";

    case TokenType::Equal:
        return "Equal";

    case TokenType::Colon:
        return "Colon";

    case TokenType::EndOfFile:
        return "EOF";
    case TokenType::Comma:
        return "Comma";
    default:
        return "Unknown";
    }

}

}