#pragma once

#include "../../vm/Bytecode.hpp"
#include "../ir/IRModule.hpp"

#include <unordered_map>
#include <string>

namespace nexus::codegen
{

class CodeGenerator
{
private:
    nexus::vm::Bytecode bytecode;
    std::unordered_map<std::string, size_t> labelOffsets;

public:
    nexus::vm::Bytecode Generate(const nexus::ir::IRModule& module);

private:
    void GenerateInstruction(const nexus::ir::Instruction& instruction);
    
    nexus::vm::Value ConvertValue(const nexus::ir::Constant& constant);
};

}
