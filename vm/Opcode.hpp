#pragma once

#include <string>
#include <variant>

namespace nexus::vm
{

enum class Opcode
{
    COMPARE,
    LABEL,
    LOAD,
    PUSH,
    STORE,
    ADD,
    SUB,
    MUL,
    DIV,
    CALL,
    JMP,
    JMP_IF_FALSE,
    LT,
    GT,
    LE,
    GE,
    EQ,
    NE,
    HALT
};

}