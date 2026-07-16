#include "../compiler/lexer/Lexer.hpp"
#include "../compiler/token/TokenStream.hpp"
#include "../compiler/parser/Parser.hpp"
#include "../compiler/semantic/SemanticAnalyzer.hpp"
#include "../compiler/ir/IRBuilder.hpp"
#include "../compiler/codegen/CodeGenerator.hpp"
#include "../vm/VirtualMachine.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

void PrintValue_vm(const nexus::vm::Value& value)
{
    std::visit(
        [](auto&& v)
        {
            std::cout << v;
        },
        value
    );
}

void PrintValue_ir(const nexus::ir::Constant& value)
{
    std::visit(
        [](auto&& v)
        {
            using T = std::decay_t<decltype(v)>;

            if constexpr (std::is_same_v<T, std::monostate>)
            {
                std::cout << "<none>";
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                std::cout << (v ? "true" : "false");
            }
            else
            {
                std::cout << v;
            }
        },
        value
    );
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: nexus <file.nx>\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    if (!file)
    {
        std::cout << "Cannot open file.\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string source = buffer.str();

    // Lexer
    nexus::lexer::Lexer lexer(source);

    std::vector<nexus::token::Token> tokens;

    while (true)
    {
        auto token = lexer.NextToken();
        tokens.push_back(token);

        if (token.type == nexus::token::TokenType::EndOfFile)
            break;
    }

    // Parser
    nexus::token::TokenStream stream(tokens);

    nexus::parser::Parser parser(std::move(stream));

    auto program = parser.Parse();

    // Semantic
    nexus::semantic::SemanticAnalyzer analyzer;
    analyzer.Analyze(*program);

    // IR
    nexus::ir::IRBuilder builder;
    auto ir = builder.Build(*program);

    // for (const auto& inst : ir.Instructions())
    // {
    //     std::cout
    //         << static_cast<int>(inst.opcode)
    //         << " ";
    //     PrintValue_ir(inst.operand);
    //     std::cout << "\n";
    // }

    // Code Generation
    nexus::codegen::CodeGenerator generator;
    auto bytecode = generator.Generate(ir);

    // 디버그 출력
    // for (const auto& inst : bytecode.Code())
    // {
    //     std::cout
    //         << static_cast<int>(inst.opcode)
    //         << " ";
    //     PrintValue_vm(inst.operand);
    //     std::cout << "\n";
    // }

    // Execute
    nexus::vm::VirtualMachine vm;
    vm.Execute(bytecode);
    
    return 0;
}