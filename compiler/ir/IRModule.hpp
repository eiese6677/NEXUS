#pragma once

#include "Instruction.hpp"

#include <vector>
#include <unordered_map>
#include <string>

namespace nexus::ir
{

struct FunctionInfo
{
    size_t address;
    std::vector<std::string> parameters;
};

class IRModule
{
    private:
    std::vector<Instruction> instructions;
    std::unordered_map<std::string, FunctionInfo> functions;

public:
    void Add(Instruction instruction)
    {
        instructions.push_back(instruction);
    }

    const std::vector<Instruction>& Instructions() const
    {
        return instructions;
    }

    void AddFunction(
        const std::string& name,
        size_t address,
        const std::vector<std::string>& parameters = {}
    )
    {
        functions[name] = {
            address,
            parameters
        };
    }

    const auto& Functions() const
    {
        return functions;
    }
};

}
