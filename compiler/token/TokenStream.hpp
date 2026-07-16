#pragma once

#include "Token.hpp"

#include <vector>
#include <cstddef>


namespace nexus::token
{

class TokenStream
{
private:

    std::vector<Token> tokens;

    size_t position;


public:

    TokenStream(const std::vector<Token>& tokens)
        : tokens(tokens),
          position(0)
    {
    }


    // 현재 토큰
    const Token& Current() const
    {
        return tokens[position];
    }


    // 다음 토큰으로 이동
    void Advance()
    {
        if (position < tokens.size() - 1)
        {
            position++;
        }
    }

    const Token& Peek(size_t offset = 1) const
    {
        size_t index = position + offset;

        if(index >= tokens.size())
            return tokens.back();

        return tokens[index];
    }

    // 현재 토큰 종류 확인
    bool Check(TokenType type) const
    {
        return Current().type == type;
    }


    // 현재 토큰 가져오고 이동
    Token Consume()
    {
        Token token = Current();

        Advance();

        return token;
    }


    // 끝인지 확인
    bool IsEnd() const
    {
        return Current().type == TokenType::EndOfFile;
    }


    size_t Position() const
    {
        return position;
    }
};

}