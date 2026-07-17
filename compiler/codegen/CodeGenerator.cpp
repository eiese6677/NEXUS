#include "CodeGenerator.hpp"
#include "../ir/Opcode.hpp"
#include "../vm/Opcode.hpp"

#include <string>
#include <variant>
#include <type_traits>
#include <stdexcept>
#include <iostream>

namespace nexus::codegen
{

// nexus::vm::Bytecode CodeGenerator::Generate(const nexus::ir::IRModule& module)
// {
//     labelOffsets.clear();
//     size_t vmIndex = 0;
//     for (const auto& instruction : module.Instructions())
//     {
//         if (instruction.opcode == nexus::ir::Opcode::Label)
//         {
//             if (!std::holds_alternative<std::string>(instruction.operand))
//             {
//                 throw std::runtime_error("Expected string operand");
//             }

//             const auto& label =
//                 std::get<std::string>(instruction.operand);

//             labelOffsets[label] = vmIndex;
//         }
//         else
//         {
//             vmIndex++;
//         }
//     }

//     bytecode = nexus::vm::Bytecode();

//         for (const auto& instruction : module.Instructions())
//     {
//         GenerateInstruction(instruction);
//     }

//     for (auto& inst : mainCode)
//     {
//         bytecode.Add(inst);
//     }

//     size_t offset = bytecode.Code().size();

//     bytecode.OffsetFunctions(offset);

//     for(auto& inst : functionCode)
//     {
//         bytecode.Add(inst);
//     }

//     std::cout << "functionAddresses size: "
//           << functionAddresses.size()
//           << "\n";

//     return bytecode;
// }
nexus::vm::Bytecode CodeGenerator::Generate(
    const nexus::ir::IRModule& module
)
{
    for(const auto& instruction : module.Instructions())
    {
        GenerateInstruction(instruction);
    }
    
    mainCode.push_back({
        vm::Opcode::HALT,
        {}
    });

    // main 코드 먼저 추가
    for(const auto& inst : mainCode)
    {
        bytecode.Add(inst);
    }


    size_t functionOffset =
        bytecode.Code().size();

    // 함수 주소 등록
    for(const auto& [name, address] : functionAddresses)
    {
        bytecode.AddFunction(
            name,
            address + functionOffset
        );
    }

    // 함수 코드 추가
    for(const auto& inst : functionCode)
    {
        bytecode.Add(inst);
    }

    return bytecode;
}

void CodeGenerator::GenerateInstruction(const nexus::ir::Instruction& instruction)
{
    switch (instruction.opcode)
    {
    case nexus::ir::Opcode::LoadConstant:
        if (inFunction)
        {
            functionCode.push_back({nexus::vm::Opcode::PUSH, ConvertValue(instruction.operand)});
        }
        else
        {
            mainCode.push_back({nexus::vm::Opcode::PUSH, ConvertValue(instruction.operand)});
        }
        break;

    case nexus::ir::Opcode::Store:
        if(inFunction)
        {
            functionCode.push_back({nexus::vm::Opcode::STORE, ConvertValue(instruction.operand)});
        }
        else
        {
            mainCode.push_back({nexus::vm::Opcode::STORE, ConvertValue(instruction.operand)});
        }

    case nexus::ir::Opcode::LoadVariable:
        if(inFunction)
        {
            functionCode.push_back({nexus::vm::Opcode::LOAD, ConvertValue(instruction.operand)});
        }
        else
        {
            mainCode.push_back({nexus::vm::Opcode::LOAD, ConvertValue(instruction.operand)});
        }
        break;

    case nexus::ir::Opcode::Add:
        if(inFunction)
        {
            functionCode.push_back({nexus::vm::Opcode::ADD, ""});
        }
        else
        {
            mainCode.push_back({nexus::vm::Opcode::ADD, ""});
        }
        break;

    case nexus::ir::Opcode::Subtract:
        if(inFunction)
        {
            functionCode.push_back({nexus::vm::Opcode::SUB, ""});
        }
        else
        {
            mainCode.push_back({nexus::vm::Opcode::SUB, ""});
        }
        break;

    case nexus::ir::Opcode::Multiply:
        if(inFunction)
        {
            functionCode.push_back({nexus::vm::Opcode::MUL, ""});
        }
        else
        {
            mainCode.push_back({nexus::vm::Opcode::MUL, ""});
        }
        break;

    case nexus::ir::Opcode::Divide:
        if(inFunction)
        {
            functionCode.push_back({nexus::vm::Opcode::DIV, ""});
        }
        else
        {
            mainCode.push_back({nexus::vm::Opcode::DIV, ""});
        }
        break;

    case nexus::ir::Opcode::Call:
        if(inFunction)
        {
            functionCode.push_back({nexus::vm::Opcode::CALL, ConvertValue(instruction.operand)});
        }
        else
        {
            mainCode.push_back({nexus::vm::Opcode::CALL, ConvertValue(instruction.operand)});
        }
        break;

    case nexus::ir::Opcode::Jump:
    {
        if (!std::holds_alternative<std::string>(instruction.operand))
        {
            throw std::runtime_error("Expected string operand");
        }

        const auto& label =
            std::get<std::string>(instruction.operand);
        if(inFunction)
        {
            functionCode.push_back({nexus::vm::Opcode::JMP,static_cast<int64_t>(labelOffsets[label])});
        }
        else
        {
            mainCode.push_back({nexus::vm::Opcode::JMP,static_cast<int64_t>(labelOffsets[label])});
        }
        break;
    }
    case nexus::ir::Opcode::JumpIfFalse:
    {
        if (!std::holds_alternative<std::string>(instruction.operand))
        {
            throw std::runtime_error("Expected string operand");
        }

        const auto& label =
            std::get<std::string>(instruction.operand);
        if(inFunction)
        {
            functionCode.push_back({nexus::vm::Opcode::JMP_IF_FALSE, static_cast<int64_t>(labelOffsets[label])});
        }
        else
        {
            mainCode.push_back({nexus::vm::Opcode::JMP_IF_FALSE, static_cast<int64_t>(labelOffsets[label])});
        }
        break;
    }
    case nexus::ir::Opcode::Compare:
    {
        if (!std::holds_alternative<std::string>(instruction.operand))
        {
            throw std::runtime_error("Expected string operand");
        }

        const auto& op =
            std::get<std::string>(instruction.operand);

        nexus::vm::Opcode vmOp = nexus::vm::Opcode::EQ;
        if (op == "<") vmOp = nexus::vm::Opcode::LT;
        else if (op == ">") vmOp = nexus::vm::Opcode::GT;
        else if (op == "<=") vmOp = nexus::vm::Opcode::LE;
        else if (op == ">=") vmOp = nexus::vm::Opcode::GE;
        else if (op == "==") vmOp = nexus::vm::Opcode::EQ;
        else if (op == "!=") vmOp = nexus::vm::Opcode::NE;
        if(inFunction)
        {
            functionCode.push_back({vmOp, ""});
        }
        else
        {
            mainCode.push_back({vmOp, ""});
        }
        break;
    }
    case ir::Opcode::Function:
    {
        currentFunction =
            std::get<std::string>(
                instruction.operand
            );

        inFunction = true;

        functionStart = functionCode.size();

        break;
    }
    case ir::Opcode::Return:
    {
        functionCode.push_back({
            vm::Opcode::RET,
            {}
        });

        functionAddresses[currentFunction] = functionStart;

        inFunction = false;
        currentFunction.clear();

        break;
    }

    default:
        break;
    }
}

nexus::vm::Value CodeGenerator::ConvertValue(
    const nexus::ir::Constant& value
)
{
    return std::visit(
        [](auto&& v) -> nexus::vm::Value
        {
            using T = std::decay_t<decltype(v)>;

            if constexpr (std::is_same_v<T, std::monostate>)
            {
                return int64_t(0);
            }
            else
            {
                return v;
            }
        },
        value
    );
}

}
