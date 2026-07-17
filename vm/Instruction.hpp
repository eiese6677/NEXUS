#pragma once

#include "Value.hpp"
#include "Opcode.hpp"

#include <cstdint>
#include <vector>
#include <string>

namespace nexus::vm
{

struct Instruction
{
    Opcode opcode;
    Value operand;
};

}