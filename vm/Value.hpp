#pragma once

#include <variant>
#include <string>
#include <iostream>
#include <type_traits>

namespace nexus::vm
{

using Value = std::variant<
    int64_t,
    double,
    std::string,
    bool
>;

inline std::ostream& operator<<(std::ostream& os, const Value& value)
{
    std::visit(
        [&](auto&& v)
        {
            using T = std::decay_t<decltype(v)>;

            if constexpr (std::is_same_v<T, bool>)
            {
                os << (v ? "true" : "false");
            }
            else
            {
                os << v;
            }
        },
        value
    );

    return os;
}

}