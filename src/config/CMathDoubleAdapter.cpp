#include "numathap/config/CMathDoubleAdapter.hpp"

#include <cmath>
#include <memory>
#include <numbers>
#include <stdexcept>

namespace numathap::config {

namespace {

using NativeType = double;
using numathap::core::Value;

inline NativeType toDouble(const Value& value) { return value.real().value(); }

inline Value makeValue(NativeType value) { return Value(value); }

void expectArguments(std::string_view function,
                     std::span<const Value> arguments, std::size_t expected) {
    if (arguments.size() != expected) {
        throw std::invalid_argument("Function '" + std::string(function) +
                                    "' expects " + std::to_string(expected) +
                                    " argument(s).");
    }
}

}  // namespace

std::string_view CMathDoubleAdapter::name() const noexcept { return "cmath"; }

std::unique_ptr<MathAdapter> CMathDoubleAdapter::clone() const {
    return std::make_unique<CMathDoubleAdapter>(*this);
}

Value CMathDoubleAdapter::callFunction(std::string_view function,
                                       std::span<const Value> arguments) const {
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

    if (function == "log2") {
        expectArguments(function, arguments, 1);
        return makeValue(std::log2(toDouble(arguments[0])));
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

    if (function == "hypot") {
        expectArguments(function, arguments, 2);
        return makeValue(
            std::hypot(toDouble(arguments[0]), toDouble(arguments[1])));
    }

    if (function == "fmod") {
        expectArguments(function, arguments, 2);
        return makeValue(
            std::fmod(toDouble(arguments[0]), toDouble(arguments[1])));
    }

    if (function == "erf") {
        expectArguments(function, arguments, 1);
        return makeValue(std::erf(toDouble(arguments[0])));
    }

    if (function == "erfc") {
        expectArguments(function, arguments, 1);
        return makeValue(std::erfc(toDouble(arguments[0])));
    }

    if (function == "tgamma") {
        expectArguments(function, arguments, 1);
        return makeValue(std::tgamma(toDouble(arguments[0])));
    }

    if (function == "lgamma") {
        expectArguments(function, arguments, 1);
        return makeValue(std::lgamma(toDouble(arguments[0])));
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
        return makeValue(
            std::pow(toDouble(arguments[0]), toDouble(arguments[1])));
    }

    if (function == "atan2") {
        expectArguments(function, arguments, 2);
        return makeValue(
            std::atan2(toDouble(arguments[0]), toDouble(arguments[1])));
    }

    throw std::invalid_argument(
        "Function '" + std::string(function) +
        "' is not available in the selected math library.");
}

Value CMathDoubleAdapter::resolveConstant(std::string_view constant) const {
    if (constant == "pi") {
        return makeValue(std::numbers::pi_v<NativeType>);
    }

    if (constant == "e") {
        return makeValue(std::numbers::e_v<NativeType>);
    }

    if (constant == "phi") {
        return makeValue(std::numbers::phi_v<NativeType>);
    }

    if (constant == "sqrt2") {
        return makeValue(std::numbers::sqrt2_v<NativeType>);
    }

    if (constant == "sqrt3") {
        return makeValue(std::numbers::sqrt3_v<NativeType>);
    }

    if (constant == "inv_pi") {
        return makeValue(std::numbers::inv_pi_v<NativeType>);
    }

    if (constant == "inv_sqrtpi") {
        return makeValue(std::numbers::inv_sqrtpi_v<NativeType>);
    }

    if (constant == "ln2") {
        return makeValue(std::numbers::ln2_v<NativeType>);
    }

    if (constant == "ln10") {
        return makeValue(std::numbers::ln10_v<NativeType>);
    }

    if (constant == "log2e") {
        return makeValue(std::numbers::log2e_v<NativeType>);
    }

    if (constant == "log10e") {
        return makeValue(std::numbers::log10e_v<NativeType>);
    }

    throw std::invalid_argument(
        "Constant '" + std::string(constant) +
        "' is not available in the selected math library.");
}

}  // namespace numathap::config