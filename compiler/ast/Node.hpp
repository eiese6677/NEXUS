#pragma once

#include <string>

namespace nexus::ast
{

class Node
{
public:

    virtual ~Node() = default;

    virtual std::string ToString() const = 0;
};

}