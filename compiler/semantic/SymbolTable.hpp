#pragma once

#include "Type.hpp"

#include <string>
#include <unordered_map>

namespace nexus::semantic
{

class SymbolTable
{
private:
    std::unordered_map<std::string, Type> symbols;

public:
    void Define(const std::string& name, Type type)
    {
        symbols[name] = type;
    }

    bool Exists(const std::string& name) const
    {
        return symbols.contains(name);
    }

    Type Get(const std::string& name) const
    {
        if (!Exists(name))
            return Type::Unknown;

        return symbols.at(name);
    }

    void Update(const std::string& name, Type type)
    {
        if (Exists(name))
        {
            symbols[name] = type;
        }
    }
};

}