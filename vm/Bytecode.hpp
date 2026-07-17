#pragma once

#include "Instruction.hpp"

#include <vector>
#include <unordered_map>
#include <iostream>

namespace nexus::vm
{

class Bytecode
{
private:

    std::vector<Instruction> code;

    std::unordered_map<
        std::string,
        size_t
    > functions;


public:

    void AddFunction(
        const std::string& name,
        size_t address
    )
    {
        // std::cout << "register function: "
        //         << name
        //         << " -> "
        //         << address
        //         << "\n";

        functions[name] = address;
    }

    size_t GetFunction(
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
        for(auto& [name, address] : functions)
        {
            address += offset;
        }
    }
};

}
