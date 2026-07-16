#pragma once

#include "../token/Token.hpp"
#include <string>

class Lexer
{
public:
    Lexer(const std::string& source);
    Token nextToken();
    
    private:
    std::string source;
    size_t position;
    int line;
    int column;
    
    private:
    char32_t currentChar() const;
    char32_t currentCodePoint() const;
    bool isDigit(char32_t ch) const;
    bool isIdentifierStart(char32_t ch) const;
    bool isIdentifierChar(char32_t ch) const;
    bool isWhitespace(char32_t ch) const;
    void advanceCodePoint();
    void advance();
    void skipWhitespace();
    Token readIdentifier();
    Token readNumber();
    Token readString();
};