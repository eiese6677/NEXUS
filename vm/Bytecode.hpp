#pragma once

#include "Instruction.hpp"

#include <vector>

namespace nexus::vm
{

class Bytecode
{
private:
    std::vector<Instruction> code;

public:
    void Add(Instruction instruction)
    {
        code.push_back(instruction);
    }

    const std::vector<Instruction>& Code() const
    {
        return code;
    }
};

}
