#include "CodeGenerator.hpp"
#include "../ir/Opcode.hpp"
#include "../vm/Opcode.hpp"
#include <string>
#include <variant>
#include <type_traits>

namespace nexus::codegen
{

nexus::vm::Bytecode CodeGenerator::Generate(const nexus::ir::IRModule& module)
{
    labelOffsets.clear();
    size_t vmIndex = 0;
    for (const auto& instruction : module.Instructions())
    {
        if (instruction.opcode == nexus::ir::Opcode::Label)
        {
            const auto& label = std::get<std::string>(instruction.operand);

            labelOffsets[label] = vmIndex;
        }
        else
        {
            vmIndex++;
        }
    }

    bytecode = nexus::vm::Bytecode();

    for (const auto& instruction : module.Instructions())
    {
        if (instruction.opcode == nexus::ir::Opcode::Label)
        {
            continue;
        }
        GenerateInstruction(instruction);
    }

    bytecode.Add({nexus::vm::Opcode::HALT, ""});
    return bytecode;
}

void CodeGenerator::GenerateInstruction(const nexus::ir::Instruction& instruction)
{
    switch (instruction.opcode)
    {
    case nexus::ir::Opcode::LoadConstant:
        bytecode.Add({nexus::vm::Opcode::PUSH, ConvertValue(instruction.operand)});
        break;

    case nexus::ir::Opcode::Store:
        bytecode.Add({nexus::vm::Opcode::STORE, ConvertValue(instruction.operand)});
        break;

    case nexus::ir::Opcode::LoadVariable:
        bytecode.Add({nexus::vm::Opcode::LOAD, ConvertValue(instruction.operand)});
        break;

    case nexus::ir::Opcode::Add:
        bytecode.Add({nexus::vm::Opcode::ADD, ""});
        break;

    case nexus::ir::Opcode::Subtract:
        bytecode.Add({nexus::vm::Opcode::SUB, ""});
        break;

    case nexus::ir::Opcode::Multiply:
        bytecode.Add({nexus::vm::Opcode::MUL, ""});
        break;

    case nexus::ir::Opcode::Divide:
        bytecode.Add({nexus::vm::Opcode::DIV, ""});
        break;

    case nexus::ir::Opcode::Call:
        bytecode.Add({nexus::vm::Opcode::CALL, ConvertValue(instruction.operand)});
        break;

    case nexus::ir::Opcode::Jump:
    {
        const auto& label = std::get<std::string>(instruction.operand);
        bytecode.Add({
            nexus::vm::Opcode::JMP,
            static_cast<int64_t>(labelOffsets[label])
        });
        break;
    }
    case nexus::ir::Opcode::JumpIfFalse:
    {
        const auto& label = std::get<std::string>(instruction.operand);
        bytecode.Add({
            nexus::vm::Opcode::JMP_IF_FALSE,
            static_cast<int64_t>(labelOffsets[label])
        });
        break;
    }
    case nexus::ir::Opcode::Compare:
    {
        const auto& op =
            std::get<std::string>(instruction.operand);

        nexus::vm::Opcode vmOp = nexus::vm::Opcode::EQ;
        if (op == "<") vmOp = nexus::vm::Opcode::LT;
        else if (op == ">") vmOp = nexus::vm::Opcode::GT;
        else if (op == "<=") vmOp = nexus::vm::Opcode::LE;
        else if (op == ">=") vmOp = nexus::vm::Opcode::GE;
        else if (op == "==") vmOp = nexus::vm::Opcode::EQ;
        else if (op == "!=") vmOp = nexus::vm::Opcode::NE;
        bytecode.Add({vmOp, ""});
        break;
    }

    default:
        break;
    }
}

nexus::vm::Value CodeGenerator::ConvertValue(
    const nexus::ir::Constant& constant)
{
    return std::visit(
        [](auto&& value) -> nexus::vm::Value
        {
            using T = std::decay_t<decltype(value)>;

            if constexpr (std::is_same_v<T, std::monostate>)
            {
                return std::string("");
            }
            else
            {
                return value;
            }
        },
        constant
    );
}

}
