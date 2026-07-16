#include "../../compiler/lexer/Lexer.hpp"
#include "../../compiler/parser/Parser.hpp"
#include "../../compiler/semantic/SemanticAnalyzer.hpp"
#include "../../compiler/ir/IRBuilder.hpp"
#include "../../compiler/codegen/CodeGenerator.hpp"
#include "../../vm/VirtualMachine.hpp"

#include <iostream>
#include <vector>

using namespace nexus::lexer;
using namespace nexus::token;
using namespace nexus::parser;
using namespace std;

void PrintValue_vm(const nexus::vm::Value& value)
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

int main() {
    Lexer lexer(
        "정수 x = 10 + 20\n"
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
    nexus::semantic::SemanticAnalyzer analyzer;
    analyzer.Analyze(program.get());

    nexus::ir::IRBuilder builder;
    auto module = builder.Build(*program);

    nexus::codegen::CodeGenerator generator;
    auto bytecode = generator.Generate(module);

    nexus::vm::VirtualMachine vm;
    vm.Execute(bytecode);

    std::cout << "AST: " << program->Statements()[0]->ToString() << "\n";
    std::cout << "Diagnostics: " << analyzer.Diagnostics().size() << "\n";
    std::cout << "IR Instructions: " << module.Instructions().size() << "\n";

    for (const auto& instruction : module.Instructions()) {
        std::cout << "IR [" << static_cast<int>(instruction.opcode) << "] ";
        PrintValue_ir(instruction.operand);
        std::cout << "\n";
    }

    std::cout << "Bytecode Instructions: " << bytecode.Code().size() << "\n";
    for (const auto& instruction : bytecode.Code()) {
        std::cout << "BC [" << static_cast<int>(instruction.opcode) << "] ";
        PrintValue_vm(instruction.operand);
        std::cout << "\n";
    }

    for (const auto& diag : analyzer.Diagnostics()) {
        std::cout << diag << "\n";
    }

    return 0;
}