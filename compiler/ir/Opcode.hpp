#pragma once

#include <string>

namespace nexus::ir
{

enum class Opcode
{
    LoadConstant,
    Store,
    LoadVariable,
    Add,
    Subtract,
    Multiply,
    Divide,
    Call,
    Jump,
    JumpIfFalse,
    Compare,
    Label,
    HALT
};

}