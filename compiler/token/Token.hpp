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

}