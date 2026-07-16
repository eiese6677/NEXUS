#pragma once

#include "Value.hpp"
#include "Opcode.hpp"

namespace nexus::vm
{

struct Instruction
{
    Opcode opcode;
    Value operand;
};

}