#include "KeywordTable.hpp"

namespace nexus::lexer
{

nexus::token::TokenType KeywordTable::find(const std::string& word)
{
    auto it = keywords.find(word);

    if (it != keywords.end())
    {
        return it->second;
    }

    return nexus::token::TokenType::Identifier;
}

}