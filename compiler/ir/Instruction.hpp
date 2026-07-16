#pragma once

#include "Opcode.hpp"

#include <variant>
#include <string>
#include <cstdint>

namespace nexus::ir
{

using Constant = std::variant<
    std::monostate,
    int64_t,
    double,
    std::string,
    bool
>;

struct Instruction
{
    nexus::ir::Opcode opcode;
    Constant operand;
};

}