#pragma once

#include "TokenType.hpp"
#include <string>

struct Token
{
    TokenType type;
    std::string value;

    int line = 0;
    int column = 0;
};