#pragma once

#include "../token/TokenType.hpp"

#include <string>
#include <unordered_map>

namespace nexus::lexer
{

class KeywordTable
{
public:

    static nexus::token::TokenType find(const std::string& word);


private:

    inline static const std::unordered_map<std::string, nexus::token::TokenType> keywords =
    {
        {"let", nexus::token::TokenType::Let},
        {"const", nexus::token::TokenType::Const},

        {"함수", nexus::token::TokenType::Function},
        {"func", nexus::token::TokenType::Function},
        {"반환", nexus::token::TokenType::Return},
        {"return", nexus::token::TokenType::Return},

        {"만약", nexus::token::TokenType::If},
        {"if", nexus::token::TokenType::If},
        {"아니면", nexus::token::TokenType::Else},
        {"else", nexus::token::TokenType::Else},
        {"그렇지않으면", nexus::token::TokenType::Elif},
        {"elif", nexus::token::TokenType::Elif},
        {"동안", nexus::token::TokenType::While},
        {"while", nexus::token::TokenType::While},

        {"출력", nexus::token::TokenType::Print},
        {"print", nexus::token::TokenType::Print},

        {"정수", nexus::token::TokenType::TypeInteger},
        {"int", nexus::token::TokenType::TypeInteger},
        {"실수", nexus::token::TokenType::TypeFloat},
        {"float", nexus::token::TokenType::TypeFloat},
        {"문자열", nexus::token::TokenType::TypeString},
        {"string", nexus::token::TokenType::TypeString},
        {"불린", nexus::token::TokenType::TypeBoolean},
        {"bool", nexus::token::TokenType::TypeBoolean},
    };

};

}