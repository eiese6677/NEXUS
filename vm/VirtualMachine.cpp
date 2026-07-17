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

void PrintValue(const nexus::vm::Value& value)
{
    std::visit(
        [](auto&& arg)
        {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (
                std::is_same_v<T, std::monostate>
            )
            {
                std::cout << "none";
            }
            else if constexpr (
                std::is_same_v<T, bool>
            )
            {
                std::cout
                    << (arg ? "true" : "false");
            }
            else
            {
                std::cout << arg;
            }

            std::cout << "\n";
        },
        value
    );
}

namespace nexus::vm
{

void VirtualMachine::Execute(const Bytecode& bytecode)
{
    stack.clear();
    globals.clear();
    callStack.clear();

    const auto& code = bytecode.Code();
    size_t pc = 0;
    
    while (pc < code.size())
    {
        const auto& instruction = code[pc];
        
        switch (instruction.opcode)
        {
        case nexus::vm::Opcode::PUSH:
            stack.push_back(instruction.operand);
            break;

        case Opcode::LOAD:
        {
            std::string name =
                std::get<std::string>(instruction.operand);

            stack.push_back(
                LoadVariable(name)
            );

            break;
        }
        case Opcode::STORE:
        {
            std::string name =
                std::get<std::string>(instruction.operand);

            auto value = stack.back();
            stack.pop_back();

            StoreVariable(name, value);

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
            std::string name =
            std::get<std::string>(instruction.operand);
            
            if(name == "출력")
            {
                auto value = stack.back();
                stack.pop_back();

                PrintValue(value);

                break;
            }

            auto& function = bytecode.GetFunction(name);


            CallFrame frame;

            frame.functionName = name;
            frame.returnAddress = pc + 1;


            for (int i = function.parameters.size() - 1;
                i >= 0;
                i--)
            {
                frame.locals[
                    function.parameters[i]
                ] = stack.back();

                stack.pop_back();
            }

            // std::cout
            //     << "STACK SIZE="
            //     << stack.size()
            //     << "\n";

            // for(auto& [k,v] : frame.locals)
            // {
            //     std::cout << k << "\n";
            // }

            callStack.push_back(frame);


            pc = function.address;

            continue;
        }

        case Opcode::RET:
        {
            if (callStack.empty())
            {
                return;
            }


            size_t returnAddress =
                callStack.back().returnAddress;


            callStack.pop_back();


            pc = returnAddress;

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

Value VirtualMachine::LoadVariable(
    const std::string& name
)
{
    if (!callStack.empty())
    {
        auto& locals = callStack.back().locals;

        auto it = locals.find(name);

        if (it != locals.end())
        {
            return it->second;
        }
    }


    auto it = globals.find(name);

    if (it != globals.end())
    {
        return it->second;
    }

    throw std::runtime_error(
        "Undefined variable: " + name
    );
}

void VirtualMachine::StoreVariable(
    const std::string& name,
    const Value& value
)
{
    if (!callStack.empty())
    {
        auto& locals = callStack.back().locals;

        if (locals.contains(name))
        {
            locals[name] = value;
            return;
        }
    }

    globals[name] = value;
}

}
