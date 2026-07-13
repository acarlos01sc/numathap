#include "numathap/config/CMathAdapter.hpp"

#include <cmath>
#include <numbers>
#include <stdexcept>

namespace numathap::config {

namespace {

using numathap::core::Value;

inline double toDouble(const Value& value) {
    return value.real().value();
}

inline Value makeValue(double value) {
    return Value(value);
}

void expectArguments(
    std::string_view function,
    std::span<const Value> arguments,
    std::size_t expected)
{
    if (arguments.size() != expected) {
        throw std::invalid_argument(
            std::string(function) +
            ": invalid number of arguments.");
    }
}

}  // namespace

std::string_view CMathAdapter::name() const noexcept {
    return "cmath";
}

Value CMathAdapter::callFunction(
    std::string_view function,
    std::span<const Value> arguments) const
{
    if (function == "sin") {
        expectArguments(function, arguments, 1);
        return makeValue(std::sin(toDouble(arguments[0])));
    }

    if (function == "cos") {
        expectArguments(function, arguments, 1);
        return makeValue(std::cos(toDouble(arguments[0])));
    }

    if (function == "tan") {
        expectArguments(function, arguments, 1);
        return makeValue(std::tan(toDouble(arguments[0])));
    }

    if (function == "asin") {
        expectArguments(function, arguments, 1);
        return makeValue(std::asin(toDouble(arguments[0])));
    }

    if (function == "acos") {
        expectArguments(function, arguments, 1);
        return makeValue(std::acos(toDouble(arguments[0])));
    }

    if (function == "atan") {
        expectArguments(function, arguments, 1);
        return makeValue(std::atan(toDouble(arguments[0])));
    }

    if (function == "sinh") {
        expectArguments(function, arguments, 1);
        return makeValue(std::sinh(toDouble(arguments[0])));
    }

    if (function == "cosh") {
        expectArguments(function, arguments, 1);
        return makeValue(std::cosh(toDouble(arguments[0])));
    }

    if (function == "tanh") {
        expectArguments(function, arguments, 1);
        return makeValue(std::tanh(toDouble(arguments[0])));
    }

    if (function == "exp") {
        expectArguments(function, arguments, 1);
        return makeValue(std::exp(toDouble(arguments[0])));
    }

    if (function == "log") {
        expectArguments(function, arguments, 1);
        return makeValue(std::log(toDouble(arguments[0])));
    }

    if (function == "log10") {
        expectArguments(function, arguments, 1);
        return makeValue(std::log10(toDouble(arguments[0])));
    }

    if (function == "sqrt") {
        expectArguments(function, arguments, 1);
        return makeValue(std::sqrt(toDouble(arguments[0])));
    }

    if (function == "cbrt") {
        expectArguments(function, arguments, 1);
        return makeValue(std::cbrt(toDouble(arguments[0])));
    }

    if (function == "abs") {
        expectArguments(function, arguments, 1);
        return makeValue(std::abs(toDouble(arguments[0])));
    }

    if (function == "floor") {
        expectArguments(function, arguments, 1);
        return makeValue(std::floor(toDouble(arguments[0])));
    }

    if (function == "ceil") {
        expectArguments(function, arguments, 1);
        return makeValue(std::ceil(toDouble(arguments[0])));
    }

    if (function == "round") {
        expectArguments(function, arguments, 1);
        return makeValue(std::round(toDouble(arguments[0])));
    }

    if (function == "pow") {
        expectArguments(function, arguments, 2);
        return makeValue(std::pow(
            toDouble(arguments[0]),
            toDouble(arguments[1])));
    }

    if (function == "atan2") {
        expectArguments(function, arguments, 2);
        return makeValue(std::atan2(
            toDouble(arguments[0]),
            toDouble(arguments[1])));
    }

    throw std::invalid_argument(
        "Unsupported function: " + std::string(function));
}

Value CMathAdapter::resolveConstant(
    std::string_view constant) const
{
    if (constant == "pi") {
        return makeValue(std::numbers::pi);
    }

    if (constant == "e") {
        return makeValue(std::numbers::e);
    }

    if (constant == "phi") {
        return makeValue(std::numbers::phi);
    }

    if (constant == "sqrt2") {
        return makeValue(std::numbers::sqrt2);
    }

    if (constant == "sqrt3") {
        return makeValue(std::numbers::sqrt3);
    }

    if (constant == "inv_pi") {
        return makeValue(std::numbers::inv_pi);
    }

    if (constant == "inv_sqrtpi") {
        return makeValue(std::numbers::inv_sqrtpi);
    }

    if (constant == "ln2") {
        return makeValue(std::numbers::ln2);
    }

    if (constant == "ln10") {
        return makeValue(std::numbers::ln10);
    }

    if (constant == "log2e") {
        return makeValue(std::numbers::log2e);
    }

    if (constant == "log10e") {
        return makeValue(std::numbers::log10e);
    }

    throw std::invalid_argument(
        "Unsupported constant: " + std::string(constant));
}

}  // namespace numathap::config