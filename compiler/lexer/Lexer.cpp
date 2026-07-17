#include "Lexer.hpp"
#include "KeywordTable.hpp"
#include "../unicode/Unicode.hpp"
#include <cctype>
#include <cstddef>

namespace nexus::lexer
{

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

nexus::token::Token Lexer::NextToken()
{
    skipWhitespace();

    // 한 줄 주석 처리
    if (currentCodePoint() == U'/')
    {
        size_t savedPosition = position;

        advanceCodePoint();

        if (currentCodePoint() == U'/')
        {
            while (currentCodePoint() != U'\n' &&
                   currentCodePoint() != U'\0')
            {
                advanceCodePoint();
            }

            return NextToken();
        }

        // 주석이 아니면 원상복구
        position = savedPosition;
    }

    int startLine = line;
    int startColumn = column;

    if (isIdentifierStart(currentCodePoint()))    {
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

    nexus::token::Token result{nexus::token::TokenType::Unknown, "",  startLine, startColumn};
    switch (currentCodePoint())
    {
    case '+':
        result = nexus::token::Token{nexus::token::TokenType::Plus,"+", startLine, startColumn};
        advanceCodePoint();
        break;
    case '-':
        result = nexus::token::Token{nexus::token::TokenType::Minus,"-", startLine, startColumn};
        advanceCodePoint();
        break;
    case '=':
        advanceCodePoint();

        if (currentCodePoint() == U'=')
        {
            advanceCodePoint();

            return nexus::token::Token{
                nexus::token::TokenType::Equal,
                "==",
                startLine,
                startColumn
            };
        }

        return nexus::token::Token{
            nexus::token::TokenType::Assign,
            "=",
            startLine,
            startColumn
        };
    case '(':
        result = nexus::token::Token{nexus::token::TokenType::LeftParen,"(", startLine, startColumn};
        advanceCodePoint();
        break;
    case ')':
        result = nexus::token::Token{nexus::token::TokenType::RightParen,")", startLine, startColumn};
        advanceCodePoint();
        break;
    case '{':
        result = nexus::token::Token{nexus::token::TokenType::LeftBrace,"{", startLine, startColumn};
        advanceCodePoint();
        break;
    case '}':
        result = nexus::token::Token{nexus::token::TokenType::RightBrace,"}", startLine, startColumn};
        advanceCodePoint();
        break;
    case '!':
        advanceCodePoint();

        if (currentCodePoint() == U'=')
        {
            advanceCodePoint();

            return nexus::token::Token{
                nexus::token::TokenType::NotEqual,
                "!=",
                startLine,
                startColumn
            };
        }

        return nexus::token::Token{
            nexus::token::TokenType::Not,
            "!",
            startLine,
            startColumn
        };
    case '>':
        advanceCodePoint();

        if(currentCodePoint() == U'=')
        {
            advanceCodePoint();

            return nexus::token::Token{
                nexus::token::TokenType::GreaterEqual,
                ">=",
                startLine,
                startColumn
            };
        }

        return nexus::token::Token{
            nexus::token::TokenType::Greater,
            ">",
            startLine,
            startColumn
        };
    case '<':
        advanceCodePoint();

        if(currentCodePoint() == U'=')
        {
            advanceCodePoint();

            return nexus::token::Token{
                nexus::token::TokenType::LessEqual,
                "<=",
                startLine,
                startColumn
            };
        }

        return nexus::token::Token{
            nexus::token::TokenType::Less,
            "<",
            startLine,
            startColumn
        };

    case '*':
        result = nexus::token::Token{
            nexus::token::TokenType::Multiply,
            "*",
            startLine,
            startColumn
        };
        advanceCodePoint();
        break;
        
    case '/':
        result = nexus::token::Token{
            nexus::token::TokenType::Divide,
            "/",
            startLine,
            startColumn
        };
        advanceCodePoint();
        break;
    case ':':
        {
            advanceCodePoint();
            return nexus::token::Token(
                nexus::token::TokenType::Colon,
                ":"
            );
        }
    case '\0':
        result = nexus::token::Token{nexus::token::TokenType::EndOfFile,"", startLine, startColumn};
        break;
    default:
        result = nexus::token::Token{
            nexus::token::TokenType::Unknown,
            Unicode::encodeUTF8(currentCodePoint()),
            startLine,
            startColumn
        };
        advanceCodePoint();
        break;
    }
    return result;
}

nexus::token::Token Lexer::readIdentifier()
{
    int startLine = line;
    int startColumn = column;

    std::string word;

    while (isIdentifierChar(currentCodePoint()))
    {
        word += Unicode::encodeUTF8(currentCodePoint());
        advanceCodePoint();
    }
    
    if(word == "참")
    {
        return nexus::token::Token{
            nexus::token::TokenType::BooleanLiteral,
            word
        };
    }

    if(word == "거짓")
    {
        return nexus::token::Token{
            nexus::token::TokenType::BooleanLiteral,
            word
        };
    }

    return nexus::token::Token{
        KeywordTable::find(word),
        word,
        startLine,
        startColumn
    };
}

nexus::token::Token Lexer::readNumber()
{
    int startLine = line;
    int startColumn = column;

    std::string number;

    bool isFloat = false;


    while (true)
    {
        char32_t ch = currentCodePoint();


        if (isDigit(ch))
        {
            number += Unicode::encodeUTF8(ch);
            advanceCodePoint();
        }
        else if (ch == U'.' && !isFloat)
        {
            // 소수점
            isFloat = true;

            number += '.';
            advanceCodePoint();
        }
        else
        {
            break;
        }
    }


    return nexus::token::Token{
        isFloat
            ? nexus::token::TokenType::FloatLiteral
            : nexus::token::TokenType::IntegerLiteral,

        number,

        startLine,
        startColumn
    };
}

nexus::token::Token Lexer::readString()
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

    return nexus::token::Token{
        nexus::token::TokenType::StringLiteral,
        word,
        startLine,
        startColumn
    };
}

}