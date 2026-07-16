#include "../compiler/lexer/Lexer.hpp"
#include "../compiler/parser/Parser.hpp"

#include <iostream>
#include <vector>

using namespace nexus::lexer;
using namespace nexus::token;
using namespace nexus::parser;
using namespace std;

std::string TokenName(TokenType type)
{
    switch(type)
    {
    case TokenType::Let: return "Let";
    case TokenType::Const: return "Const";

    case TokenType::Print: return "Print";

    case TokenType::Identifier: return "Identifier";

    case TokenType::IntegerLiteral: return "IntegerLiteral";
    case TokenType::StringLiteral: return "StringLiteral";
    
    case TokenType::TypeInteger: return "TypeInteger";
    
    case TokenType::Assign: return "Assign";

    case TokenType::Plus: return "Plus";
    case TokenType::Minus: return "Minus";

    case TokenType::LeftParen: return "LeftParen";
    case TokenType::RightParen: return "RightParen";

    case TokenType::EndOfFile: return "EndOfFile";

    default:
        return "Unknown";
    }
}


int main()
{
    Lexer lexer(
        "정수 x = 10"
    );


    vector<Token> tokens;


    while(true)
    {
        auto token = lexer.NextToken();

        tokens.push_back(token);

        if(token.type == TokenType::EndOfFile)
            break;
    }


    TokenStream stream(tokens);


    Parser parser(stream);


    auto program = parser.Parse();


    cout
        << program->Statements()[0]->ToString();
    return 0;
}