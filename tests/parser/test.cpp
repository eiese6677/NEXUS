#include "../compiler/lexer/Lexer.hpp"
#include "../compiler/parser/Parser.hpp"

#include <iostream>
#include <vector>

using namespace nexus::lexer;
using namespace nexus::token;
using namespace nexus::parser;
using namespace std;

int main() {
    Lexer lexer(
        "출력(x)"
    );

    std::vector<Token> tokens;

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

    std::cout 
        << program->Statements()[0]->ToString();
    
    return 0;
}