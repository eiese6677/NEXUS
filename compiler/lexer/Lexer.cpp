#include "Lexer.hpp"
#include "KeywordTable.hpp"
#include "../unicode/Unicode.hpp"
#include <cctype>

Lexer::Lexer(const std::string& source)
    : source(source),
      position(0),
      line(1),
      column(1)
{

}

bool Lexer::isDigit(char32_t ch) const
{
    return ch >= U'0' && ch <= U'9';
}


bool Lexer::isIdentifierStart(char32_t ch) const
{
    return
        (ch >= U'a' && ch <= U'z') ||
        (ch >= U'A' && ch <= U'Z') ||
        (ch >= U'가' && ch <= U'힣') ||
        ch == U'_';
}


bool Lexer::isIdentifierChar(char32_t ch) const
{
    return isIdentifierStart(ch) || isDigit(ch);
}

bool Lexer::isWhitespace(char32_t ch) const
{
    return ch == U' ' ||
           ch == U'\t' ||
           ch == U'\n' ||
           ch == U'\r';
}

char32_t Lexer::currentCodePoint() const
{
    if (position >= source.size())
        return U'\0';

    unsigned char c = source[position];

    // ASCII
    if (c < 0x80)
    {
        return c;
    }

    // 2 byte
    if ((c & 0xE0) == 0xC0)
    {
        return
            ((c & 0x1F) << 6) |
            (source[position + 1] & 0x3F);
    }

    // 3 byte
    if ((c & 0xF0) == 0xE0)
    {
        return
            ((c & 0x0F) << 12) |
            ((source[position + 1] & 0x3F) << 6) |
            (source[position + 2] & 0x3F);
    }

    // 4 byte
    if ((c & 0xF8) == 0xF0)
    {
        return
            ((c & 0x07) << 18) |
            ((source[position + 1] & 0x3F) << 12) |
            ((source[position + 2] & 0x3F) << 6) |
            (source[position + 3] & 0x3F);
    }

    return U'\0';
}

void Lexer::advanceCodePoint()
{
    if (position >= source.size())
        return;

    char32_t ch = currentCodePoint();

    if (ch == U'\n')
    {
        line++;
        column = 1;
    }
    else
    {
        column++;
    }

    unsigned char c = source[position];

    if (c < 0x80)
    {
        position += 1;
    }
    else if ((c & 0xE0) == 0xC0)
    {
        position += 2;
    }
    else if ((c & 0xF0) == 0xE0)
    {
        position += 3;
    }
    else if ((c & 0xF8) == 0xF0)
    {
        position += 4;
    }
}

void Lexer::skipWhitespace()
{
    while (isWhitespace(currentCodePoint()))
    {
        advanceCodePoint();
    }
}

Token Lexer::nextToken()
{
    skipWhitespace();

    int startLine = line;
    int startColumn = column;

    if (isIdentifierStart(currentCodePoint()))
    {
        return readIdentifier();
    }
    
    if (isDigit(currentCodePoint()))
    {
        return readNumber();
    }

    if (currentCodePoint() == '\"')
    {
        return readString();
    }
    
    Token result{TokenType::Unknown, "",  startLine, startColumn};
    switch (currentCodePoint())
    {
    case '+':
        result = Token{TokenType::Plus,"+", startLine, startColumn};
        advanceCodePoint();
        break;
    case '-':
        result = Token{TokenType::Minus,"-", startLine, startColumn};
        advanceCodePoint();
        break;
    case '=':
        result = Token{TokenType::Assign,"=", startLine, startColumn};
        advanceCodePoint();
        break;
    case '(':
        result = Token{TokenType::LeftParen,"(", startLine, startColumn};
        advanceCodePoint();
        break;
    case ')':
        result = Token{TokenType::RightParen,")", startLine, startColumn};
        advanceCodePoint();
        break;
    case '{':
        result = Token{TokenType::LeftBrace,"{", startLine, startColumn};
        advanceCodePoint();
        break;
    case '}':
        result = Token{TokenType::RightBrace,"}", startLine, startColumn};
        advanceCodePoint();
        break;
    case '\0':
        result = Token{TokenType::EndOfFile,"", startLine, startColumn};
        break;
    }
    return result;
}

Token Lexer::readIdentifier()
{
    int startLine = line;
    int startColumn = column;

    std::string word;

    while (isIdentifierChar(currentCodePoint()))
    {
        word += Unicode::encodeUTF8(currentCodePoint());
        advanceCodePoint();
    }

    return Token{
        KeywordTable::find(word),
        word,
        startLine,
        startColumn
    };
}

Token Lexer::readNumber()
{
    int startLine = line;
    int startColumn = column;

    std::string number;


    while (isDigit(currentCodePoint()))
    {
        number += Unicode::encodeUTF8(currentCodePoint());
        advanceCodePoint();
    }


    return Token{
        TokenType::IntegerLiteral,
        number,
        startLine,
        startColumn
    };
}

Token Lexer::readString()
{
    int startLine = line;
    int startColumn = column;

    std::string word;

    advanceCodePoint();

    while (currentCodePoint() != U'"' &&
           currentCodePoint() != U'\0')
    {
        word += Unicode::encodeUTF8(currentCodePoint());
        advanceCodePoint();
    }

    if (currentCodePoint() == U'"')
    {
        advanceCodePoint();
    }

    return Token{
        TokenType::StringLiteral,
        word,
        startLine,
        startColumn
    };
}