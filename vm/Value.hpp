#pragma once

#include <variant>
#include <string>
#include <cstdint>

namespace nexus::vm
{

using Value = std::variant<
    std::monostate,
    int64_t,
    double,
    std::string,
    bool
>;


Value Add(const Value& a, const Value& b);
Value Subtract(const Value& a, const Value& b);
Value Multiply(const Value& a, const Value& b);
Value Divide(const Value& a, const Value& b);

bool CompareEqual(const Value& a, const Value& b);

std::string ToString(const Value& value);

}