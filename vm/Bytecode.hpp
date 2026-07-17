#pragma once

#include "Instruction.hpp"

#include <vector>
#include <unordered_map>
#include <iostream>

struct FunctionInfo
{
    size_t address;
    std::vector<std::string> parameters;
};

namespace nexus::vm
{

class Bytecode
{
private:

    std::vector<Instruction> code;
    std::unordered_map<std::string, FunctionInfo> functions;

public:

    void AddFunction(
        const std::string& name,
        size_t address,
        const std::vector<std::string>& parameters
    )
    {
        functions[name] = {
            address,
            parameters
        };
    }

    const FunctionInfo& GetFunction(
        const std::string& name
    ) const
    {
        return functions.at(name);
    }
    
    void Add(Instruction instruction)
    {
        code.push_back(instruction);
    }

    const std::vector<Instruction>& Code() const
    {
        return code;
    }

    bool HasFunction(const std::string& name) const
    {
        return functions.contains(name);
    }
    const auto& Functions() const
    {
        return functions;
    }
    void OffsetFunctions(size_t offset)
    {
        for (auto& [name, function] : functions)
        {
            function.address += offset;
        }
    }
};

}
