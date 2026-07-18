#pragma once

#include "Type.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace nexus::semantic
{

struct Symbol
{
    Type type = Type::Unknown;
    bool isConst = false;
};

class SymbolTable
{
private:
    std::vector<std::unordered_map<std::string, Symbol>> scopes;
public:

    void Define(
        const std::string& name,
        Type type,
        bool isConst = false)
    {
        scopes.back()[name] = {
            type,
            isConst
        };
    }
    
    bool Exists(const std::string& name) const
    {
        for (auto it = scopes.rbegin();
        it != scopes.rend();
        ++it)
        {
            if (it->contains(name))
            return true;
        }
        
        return false;
    }
    
    Symbol Get(const std::string& name) const
    {
        for (auto it = scopes.rbegin();
        it != scopes.rend();
        ++it)
        {
            auto found = it->find(name);
            
            if (found != it->end())
            return found->second;
        }
        
        return {};
    }
    
    void Clear()
    {
        scopes.clear();
        PushScope();
    }
    
    void PushScope()
    {
        scopes.emplace_back();
    }
    
    void PopScope()
    {
        if (!scopes.empty())
        scopes.pop_back();
    }
    
    bool ExistsInCurrentScope(const std::string& name) const
    {
        return scopes.back().contains(name);
    }

    void Update(
        const std::string& name,
        Type type)
    {
        for (auto it = scopes.rbegin();
            it != scopes.rend();
            ++it)
        {
            auto found = it->find(name);

            if (found != it->end())
            {
                found->second.type = type;
                return;
            }
        }
    }

    SymbolTable()
    {
        PushScope();
    }
};  
    
}