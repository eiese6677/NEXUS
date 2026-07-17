#pragma once

#include <string>

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
    HALT,
    RET,
};


inline std::string OpcodeToString(Opcode opcode)
{
    switch(opcode)
    {
        case Opcode::ADD:
            return "ADD";
        case Opcode::CALL:
            return "CALL";
        case Opcode::COMPARE:
            return "COMPARE";
        case Opcode::DIV:
            return "DIV";
        case Opcode::EQ:
            return "EQ";
        case Opcode::GE:
            return "GE";
        case Opcode::GT:
            return "GT";
        case Opcode::HALT:
            return "HALT";
        case Opcode::JMP:
            return "JMP";
        case Opcode::JMP_IF_FALSE:
            return "JMP_IF_FALSE";
        case Opcode::LABEL:
            return "LABEL";
        case Opcode::LE:
            return "LE";
        case Opcode::LOAD:
            return "LOAD";
        case Opcode::LT:
            return "LT";
        case Opcode::MUL:
            return "MUL";
        case Opcode::NE:
            return "NE";
        case Opcode::PUSH:
            return "PUSH";
        case Opcode::STORE:
            return "STORE";
        case Opcode::SUB:
            return "SUB";
        case Opcode::RET:
            return "RET";
    }

    return "UNKNOWN";
}

}