#include "Unicode.hpp"


namespace Unicode
{

std::string encodeUTF8(char32_t cp)
{
    std::string result;


    if (cp <= 0x7F)
    {
        result += static_cast<char>(cp);
    }
    else if (cp <= 0x7FF)
    {
        result += static_cast<char>(0xC0 | (cp >> 6));
        result += static_cast<char>(0x80 | (cp & 0x3F));
    }
    else if (cp <= 0xFFFF)
    {
        result += static_cast<char>(0xE0 | (cp >> 12));
        result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (cp & 0x3F));
    }
    else
    {
        result += static_cast<char>(0xF0 | (cp >> 18));
        result += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
        result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (cp & 0x3F));
    }


    return result;
}

}