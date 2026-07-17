#include "VirtualMachine.hpp"
#include "Value.hpp"
#include "Instruction.hpp"
#include "Opcode.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

double ToNumber(const nexus::vm::Value& v)
{
    if (std::holds_alternative<int64_t>(v))
        return static_cast<double>(
            std::get<int64_t>(v)
        );

    if (std::holds_alternative<double>(v))
        return std::get<double>(v);

    throw std::runtime_error("not a number");
}

namespace nexus::vm
{

void VirtualMachine::Execute(const Bytecode& bytecode)
{
    stack.clear();
    variables.clear();
    callStack.clear();

    const auto& code = bytecode.Code();
    size_t pc = 0;

    // std::cout 
    //     << "function count: "
    //     << bytecode.Functions().size()
    //     << "\n";
    
    while (pc < code.size())
    {
        const auto& instruction = code[pc];

        switch (instruction.opcode)
        {
        case nexus::vm::Opcode::PUSH:
            stack.push_back(instruction.operand);
            break;

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

        case Opcode::CALL:
        {
            auto name =
            std::get<std::string>(
                instruction.operand
            );
            
            // std::cout
            //     << "CALL "
            //     << name
            //     << " from "
            //     << pc
            //     << "\n";

            if(name == "출력")
            {
                auto value = stack.back();
                stack.pop_back();

                std::cout << ToString(value) << "\n";
                break;
            }


            if(bytecode.Functions().contains(name))
            {
                callStack.push_back(pc + 1);

                pc = bytecode.GetFunction(name);

                continue;
            }

            throw std::runtime_error(
                "undefined function: " + name
            );
        }

        case Opcode::RET:
        {
            // std::cout 
            //     << "RET pc="
            //     << pc
            //     << " return="
            //     << callStack.back()
            //     << "\n";
            
            if(callStack.empty())
                throw std::runtime_error(
                    "call stack underflow"
                );

            pc = callStack.back();
            callStack.pop_back();

            continue;
        }

        case nexus::vm::Opcode::JMP:
            pc = static_cast<size_t>(std::get<int64_t>(instruction.operand));
            continue;

        case Opcode::JMP_IF_FALSE:
        {
            if(stack.empty())
                throw std::runtime_error("stack underflow on JMP_IF_FALSE");

            Value cond = stack.back();
            stack.pop_back();

            bool result = false;

            if(std::holds_alternative<bool>(cond))
            {
                result = std::get<bool>(cond);
            }
            else if(std::holds_alternative<int64_t>(cond))
            {
                result = std::get<int64_t>(cond) != 0;
            }
            else
            {
                throw std::runtime_error(
                    "invalid condition type"
                );
            }

            if(!result)
            {
                pc = static_cast<size_t>(
                    std::get<int64_t>(instruction.operand)
                );
                continue;
            }

            break;
        }

        case Opcode::LT:
        {
            auto b = stack.back();
            stack.pop_back();

            auto a = stack.back();
            stack.pop_back();

            stack.push_back(
                ToNumber(a) < ToNumber(b)
            );

            break;
        }
        case nexus::vm::Opcode::GT:
        {
            auto b = stack.back();
            stack.pop_back();

            auto a = stack.back();
            stack.pop_back();

            stack.push_back(
                ToNumber(a) > ToNumber(b)
            );

            break;
        }

        case nexus::vm::Opcode::LE:
        {
            auto b = stack.back();
            stack.pop_back();

            auto a = stack.back();
            stack.pop_back();

            stack.push_back(
                ToNumber(a) <= ToNumber(b)
            );

            break;
        }

        case nexus::vm::Opcode::GE:
        {
            auto b = stack.back();
            stack.pop_back();

            auto a = stack.back();
            stack.pop_back();

            stack.push_back(
                ToNumber(a) <= ToNumber(b)
            );

            break;
        }

        case nexus::vm::Opcode::EQ:
        {
            auto b = stack.back();
            stack.pop_back();

            auto a = stack.back();
            stack.pop_back();

            stack.push_back(
                ToNumber(a) == ToNumber(b)
            );

            break;
        }

        case nexus::vm::Opcode::NE:
        {
            auto b = stack.back();
            stack.pop_back();

            auto a = stack.back();
            stack.pop_back();

            stack.push_back(
                ToNumber(a) != ToNumber(b)
            );

            break;
        }

        case nexus::vm::Opcode::HALT:
            return;
        }

        pc++;
    }
}

}
