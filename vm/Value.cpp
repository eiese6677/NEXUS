#include "Value.hpp"

#include <stdexcept>
#include <type_traits>

namespace nexus::vm
{

static double ToNumber(const Value& value)
{
    if (std::holds_alternative<int64_t>(value))
        return static_cast<double>(
            std::get<int64_t>(value)
        );

    if (std::holds_alternative<double>(value))
        return std::get<double>(value);


    throw std::runtime_error(
        "value is not number"
    );
}

Value Add(const Value& a, const Value& b)
{
    if (std::holds_alternative<int64_t>(a) &&
        std::holds_alternative<int64_t>(b))
    {
        return std::get<int64_t>(a)
             + std::get<int64_t>(b);
    }


    if (std::holds_alternative<std::string>(a) &&
        std::holds_alternative<std::string>(b))
    {
        return std::get<std::string>(a)
             + std::get<std::string>(b);
    }


    return ToNumber(a) + ToNumber(b);
}

Value Subtract(const Value& a, const Value& b)
{
    return ToNumber(a) - ToNumber(b);
}

Value Multiply(const Value& a, const Value& b)
{
    return ToNumber(a) * ToNumber(b);
}

Value Divide(const Value& a, const Value& b)
{
    double divisor = ToNumber(b);

    if (divisor == 0)
        throw std::runtime_error(
            "division by zero"
        );

    return ToNumber(a) / divisor;
}

bool CompareEqual(const Value& a, const Value& b)
{
    if (a.index() != b.index())
        return false;

    return a == b;
}

std::string ToString(const Value& value)
{
    return std::visit(
        [](auto&& v) -> std::string
        {
            using T = std::decay_t<decltype(v)>;

            if constexpr (std::is_same_v<T, bool>)
            {
                return v ? "true" : "false";
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                return v;
            }
            else
            {
                return std::to_string(v);
            }
        },
        value
    );
}

}