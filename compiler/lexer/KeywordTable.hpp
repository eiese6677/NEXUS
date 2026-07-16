#pragma once

#include "../token/TokenType.hpp"

#include <string>
#include <unordered_map>


class KeywordTable
{
public:
    static TokenType find(const std::string& word)
    {
        auto it = keywords.find(word);

        if (it != keywords.end())
            return it->second;

        return TokenType::Identifier;
    }


private:
    inline static const std::unordered_map<std::string, TokenType> keywords =
    {
        {"let", TokenType::Let},
        {"const", TokenType::Const},

        {"출력", TokenType::Print},

        {"정수", TokenType::TypeInteger},
        {"실수", TokenType::TypeFloat},
        {"문자열", TokenType::TypeString},
        {"불린", TokenType::TypeBoolean}
    };
};