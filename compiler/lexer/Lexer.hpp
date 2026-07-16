#pragma once

#include "../token/Token.hpp"
#include <string>
#include <cstddef>

namespace nexus::lexer
{

class Lexer
{
public:
    Lexer(const std::string& source);
    nexus::token::Token NextToken();
    
    private:
    std::string source;
    size_t position;
    int line;
    int column;
    
    char32_t currentCodePoint() const;
    bool isDigit(char32_t ch) const;
    bool isIdentifierStart(char32_t ch) const;
    bool isIdentifierChar(char32_t ch) const;
    bool isWhitespace(char32_t ch) const;
    void advanceCodePoint();
    void skipWhitespace();
    nexus::token::Token readIdentifier();
    nexus::token::Token readNumber();
    nexus::token::Token readString();
};

}