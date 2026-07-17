#pragma once

#include "Instruction.hpp"

#include <vector>
#include <unordered_map>
#include <string>

namespace nexus::ir
{

class IRModule
{
    private:
    std::vector<Instruction> instructions;
    std::unordered_map<std::string, size_t> functions;

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
        size_t address
    )
    {
        functions[name] = address;
    }

    size_t GetFunction(
        const std::string& name
    ) const
    {
        return functions.at(name);
    }
};

}
