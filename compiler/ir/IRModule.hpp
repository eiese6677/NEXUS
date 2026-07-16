#pragma once

#include "Instruction.hpp"

#include <vector>

namespace nexus::ir
{

class IRModule
{
private:
    std::vector<Instruction> instructions;

public:
    void Add(Instruction instruction)
    {
        instructions.push_back(instruction);
    }

    const std::vector<Instruction>& Instructions() const
    {
        return instructions;
    }
};

}
