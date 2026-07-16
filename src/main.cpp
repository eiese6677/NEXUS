#include "../compiler/lexer/Lexer.hpp"
#include <iostream>


int main()
{
    std::string code = R"(
        let 이름: 정수 = 123
        출력("Hello")
    )";


    Lexer lexer(code);

    while(true)
    {
        Token token = lexer.nextToken();

        std::cout
            << token.value
            << "\n";


        if(token.type == TokenType::EndOfFile)
            break;
    }
}