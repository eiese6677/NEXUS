#include "VirtualMachine.hpp"
#include "Value.hpp"
#include "Instruction.hpp"
#include "Opcode.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

using Operand = std::variant<
    std::monostate,
    int64_t,
    double,
    std::string,
    bool
>;
using Value = Operand;

namespace nexus::vm
{

void VirtualMachine::Execute(const Bytecode& bytecode)
{
    const auto& code = bytecode.Code();
    size_t pc = 0;
    
    while (pc < code.size())
    {
        const auto& instruction = code[pc];
        pc++;

        switch (instruction.opcode)
        {
        case nexus::vm::Opcode::PUSH:
            stack.push_back(instruction.operand);
            break;

        // case Opcode::PUSH:
        // {
        //     std::cout << "PUSH\n";

        //     std::visit([](auto&& v){
        //         std::cout << typeid(v).name() << "\n";
        //     }, instruction.operand);

        //     stack.push_back(instruction.operand);
        //     break;
        // }

        case nexus::vm::Opcode::LOAD:
        {
            if (!variables.contains(std::get<std::string>(instruction.operand)))
                throw std::runtime_error("undefined variable: " + std::get<std::string>(instruction.operand));
            stack.push_back(variables[std::get<std::string>(instruction.operand)]);
            break;
        }

        case nexus::vm::Opcode::STORE:
        {
            if (stack.empty())
                throw std::runtime_error("stack underflow on STORE");
            variables[std::get<std::string>(instruction.operand)] = stack.back();
            stack.pop_back();
            break;
        }

        case Opcode::ADD:
        {
            auto b = stack.back();
            stack.pop_back();

            auto a = stack.back();
            stack.pop_back();

            stack.push_back(
                Add(a,b)
            );

            break;
        }
        
        case Opcode::SUB:
        {
            auto b = stack.back();
            stack.pop_back();

            auto a = stack.back();
            stack.pop_back();

            stack.push_back(
                Subtract(a,b)
            );

            break;
        }

        case nexus::vm::Opcode::MUL:
        {
            auto b = stack.back();
            stack.pop_back();

            auto a = stack.back();
            stack.pop_back();

            stack.push_back(
                Multiply(a,b)
            );

            break;
        }

        case nexus::vm::Opcode::DIV:
        {
            auto b = stack.back();
            stack.pop_back();

            auto a = stack.back();
            stack.pop_back();

            stack.push_back(
                Divide(a,b)
            );

            break;
        }

        case nexus::vm::Opcode::CALL:
            if (std::get<std::string>(instruction.operand) == "출력")
            {
                if (stack.empty())
                    throw std::runtime_error("stack underflow on CALL 출력");
                
                    std::visit([](const auto& value)
                    {
                        using T = std::decay_t<decltype(value)>;

                        if constexpr (std::is_same_v<T, std::monostate>)
                        {
                            std::cout << "null";
                        }
                        else
                        {
                            std::cout << value;
                        }
                    }, stack.back());
                std::cout << '\n';

                stack.pop_back();
            }
            else
            {
                std::cout << "[CALL] " << std::get<std::string>(instruction.operand) << "\n";
            }
            break;

        case nexus::vm::Opcode::JMP:
            pc = static_cast<size_t>(std::get<int64_t>(instruction.operand));
            break;

        case nexus::vm::Opcode::JMP_IF_FALSE:
        {
            if (stack.empty())
                throw std::runtime_error("stack underflow on JMP_IF_FALSE");
            const double val = std::get<double>(stack.back());
            stack.pop_back();
            if (val == 0.0)
            {
                pc = static_cast<size_t>(std::get<int64_t>(instruction.operand));
            }
            break;
        }

        case nexus::vm::Opcode::LT:
        {
            if (stack.size() < 2)
                throw std::runtime_error("stack underflow on LT");
            const double b = std::get<double>(stack.back());
            stack.pop_back();
            const double a = std::get<double>(stack.back());
            stack.pop_back();
            stack.push_back(a < b ? 1.0 : 0.0);
            break;
        }

        case nexus::vm::Opcode::GT:
        {
            if (stack.size() < 2)
                throw std::runtime_error("stack underflow on GT");
            const double b = std::get<double>(stack.back());
            stack.pop_back();
            const double a = std::get<double>(stack.back());
            stack.pop_back();
            stack.push_back(a > b ? 1.0 : 0.0);
            break;
        }

        case nexus::vm::Opcode::LE:
        {
            if (stack.size() < 2)
                throw std::runtime_error("stack underflow on LE");
            const double b = std::get<double>(stack.back());
            stack.pop_back();
            const double a = std::get<double>(stack.back());
            stack.pop_back();
            stack.push_back(a <= b ? 1.0 : 0.0);
            break;
        }

        case nexus::vm::Opcode::GE:
        {
            if (stack.size() < 2)
                throw std::runtime_error("stack underflow on GE");
            const double b = std::get<double>(stack.back());
            stack.pop_back();
            const double a = std::get<double>(stack.back());
            stack.pop_back();
            stack.push_back(a >= b ? 1.0 : 0.0);
            break;
        }

        case nexus::vm::Opcode::EQ:
        {
            if (stack.size() < 2)
                throw std::runtime_error("stack underflow on EQ");
            const double b = std::get<double>(stack.back());
            stack.pop_back();
            const double a = std::get<double>(stack.back());
            stack.pop_back();
            stack.push_back(a == b ? 1.0 : 0.0);
            break;
        }

        case nexus::vm::Opcode::NE:
        {
            if (stack.size() < 2)
                throw std::runtime_error("stack underflow on NE");
            const double b = std::get<double>(stack.back());
            stack.pop_back();
            const double a = std::get<double>(stack.back());
            stack.pop_back();
            stack.push_back(a != b ? 1.0 : 0.0);
            break;
        }

        case nexus::vm::Opcode::HALT:
            return;
        }
    }
}

}
