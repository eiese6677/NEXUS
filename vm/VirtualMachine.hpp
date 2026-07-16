#pragma once

#include "Bytecode.hpp"
#include "Value.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace nexus::vm
{

class VirtualMachine
{
private:
    std::vector<Value> stack;
    std::unordered_map<std::string, Value> variables;

public:
    void Execute(const Bytecode& bytecode);
};

}
