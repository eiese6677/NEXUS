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
    std::string currentFunction;
    size_t functionStart = 0;
    std::vector<nexus::vm::Instruction> mainCode;
    std::vector<nexus::vm::Instruction> functionCode;
    
    bool inFunction = false;

public:
    nexus::vm::Bytecode Generate(const nexus::ir::IRModule& module);

private:
    void GenerateInstruction(const nexus::ir::Instruction& instruction);
    void ResolveLabels(const nexus::ir::IRModule& module);
    nexus::vm::Value ConvertValue(const nexus::ir::Constant& constant);
    std::unordered_map<std::string, size_t> functionAddresses;
    std::unordered_map<std::string, size_t> generatedFunctions;
};

}
