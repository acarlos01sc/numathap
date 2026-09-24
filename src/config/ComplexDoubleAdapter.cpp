/**
 * @file ComplexDoubleAdapter.cpp
 *
 * @brief Implementation of the ComplexDoubleAdapter class.
 */

#include "numathap/config/ComplexDoubleAdapter.hpp"

#include <cmath>
#include <complex>
#include <limits>
#include <memory>
#include <numbers>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>

namespace numathap::config {

namespace {

using NativeComplex = std::complex<double>;

constexpr double TWO_PI = 2.0 * std::numbers::pi_v<double>;

/**
 * @brief Converts a numathap Complex value to std::complex<double>.
 */
NativeComplex toNative(const core::Value& value)
{
    if (!value.isComplex()) {
        throw std::invalid_argument(
            "ComplexDoubleAdapter expects a complex argument.");
    }

    const numeric::Complex& value_complex = value.complex();

    return NativeComplex(
        value_complex.real().value(),
        value_complex.imaginary().value());
}

/**
 * @brief Converts a std::complex<double> to a numathap Complex value.
 */
core::Value fromNative(const NativeComplex& value)
{
    return core::Value(
        numeric::Complex(
            numeric::Real(value.real()),
            numeric::Real(value.imag())));
}

/**
 * @brief Checks the number of arguments passed to a function.
 */
void expectArguments(
    std::string_view function,
    std::span<const core::Value> arguments,
    std::size_t expected)
{
    if (arguments.size() != expected) {
        throw std::invalid_argument(
            "Function '" + std::string(function) +
            "' expects " + std::to_string(expected) +
            " argument(s).");
    }
}

/**
 * @brief Extracts the branch index k from a real Value.
 *
 * The branch index must be a finite integer.
 */
int branchIndex(
    std::string_view function,
    const core::Value& value)
{
    if (!value.isReal()) {
        throw std::invalid_argument(
            "Branch index k of function '" +
            std::string(function) +
            "' must be real.");
    }

    const double k = value.real().value();

    if (!std::isfinite(k) || std::floor(k) != k) {
        throw std::invalid_argument(
            "Branch index k of function '" +
            std::string(function) +
            "' must be a finite integer.");
    }

    if (k < static_cast<double>(std::numeric_limits<int>::min()) ||
        k > static_cast<double>(std::numeric_limits<int>::max())) {
        throw std::invalid_argument(
            "Branch index k of function '" +
            std::string(function) +
            "' is outside the supported range.");
    }

    return static_cast<int>(k);
}

/**
 * @brief Computes the k-th logarithm branch.
 *
 * log_k(z) = Log(z) + 2*pi*i*k
 */
NativeComplex logarithm(
    const NativeComplex& z,
    int k)
{
    return std::log(z) +
           NativeComplex(0.0, TWO_PI * static_cast<double>(k));
}

/**
 * @brief Computes the k-th square-root branch.
 *
 * sqrt_k(z) alternates between the two square-root branches.
 */
NativeComplex squareRoot(
    const NativeComplex& z,
    int k)
{
    NativeComplex result = std::sqrt(z);

    if (k % 2 != 0) {
        result = -result;
    }

    return result;
}

/**
 * @brief Computes the k-th power branch.
 *
 * z^w = exp(w * (Log(z) + 2*pi*i*k))
 */
NativeComplex power(
    const NativeComplex& base,
    const NativeComplex& exponent,
    int k)
{
    return std::exp(
        exponent * logarithm(base, k));
}

} // namespace

std::string_view ComplexDoubleAdapter::name() const noexcept
{
    return "complex-double";
}

core::Value ComplexDoubleAdapter::callFunction(
    std::string_view function,
    std::span<const core::Value> arguments) const
{
    /*
     * Elementary functions.
     *
     * std::complex provides their principal-value definitions.
     */

    if (function == "exp") {
        expectArguments(function, arguments, 1);
        return fromNative(std::exp(toNative(arguments[0])));
    }

    if (function == "log") {
        if (arguments.size() == 1) {
            return fromNative(
                logarithm(toNative(arguments[0]), 0));
        }

        if (arguments.size() == 2) {
            const int k = branchIndex(function, arguments[1]);

            return fromNative(
                logarithm(toNative(arguments[0]), k));
        }

        throw std::invalid_argument(
            "Function 'log' expects 1 or 2 argument(s).");
    }

    if (function == "log10") {
        expectArguments(function, arguments, 1);
        return fromNative(
            std::log(toNative(arguments[0])) /
            std::log(10.0));
    }

    if (function == "sqrt") {
        if (arguments.size() == 1) {
            return fromNative(
                squareRoot(toNative(arguments[0]), 0));
        }

        if (arguments.size() == 2) {
            const int k = branchIndex(function, arguments[1]);

            return fromNative(
                squareRoot(toNative(arguments[0]), k));
        }

        throw std::invalid_argument(
            "Function 'sqrt' expects 1 or 2 argument(s).");
    }

    /*
     * Power.
     *
     * The third argument is the branch index:
     *
     *     pow(z, w)    -> principal branch
     *     pow(z, w, k) -> k-th branch
     */
    if (function == "pow") {
        if (arguments.size() == 2) {
            return fromNative(
                power(
                    toNative(arguments[0]),
                    toNative(arguments[1]),
                    0));
        }

        if (arguments.size() == 3) {
            const int k = branchIndex(function, arguments[2]);

            return fromNative(
                power(
                    toNative(arguments[0]),
                    toNative(arguments[1]),
                    k));
        }

        throw std::invalid_argument(
            "Function 'pow' expects 2 or 3 argument(s).");
    }

    /*
     * Trigonometric functions.
     *
     * std::complex supplies the principal values.
     */
    if (function == "sin") {
        expectArguments(function, arguments, 1);
        return fromNative(std::sin(toNative(arguments[0])));
    }

    if (function == "cos") {
        expectArguments(function, arguments, 1);
        return fromNative(std::cos(toNative(arguments[0])));
    }

    if (function == "tan") {
        expectArguments(function, arguments, 1);
        return fromNative(std::tan(toNative(arguments[0])));
    }

    if (function == "asin") {
        expectArguments(function, arguments, 1);
        return fromNative(std::asin(toNative(arguments[0])));
    }

    if (function == "acos") {
        expectArguments(function, arguments, 1);
        return fromNative(std::acos(toNative(arguments[0])));
    }

    if (function == "atan") {
        expectArguments(function, arguments, 1);
        return fromNative(std::atan(toNative(arguments[0])));
    }

    /*
     * Hyperbolic functions.
     */
    if (function == "sinh") {
        expectArguments(function, arguments, 1);
        return fromNative(std::sinh(toNative(arguments[0])));
    }

    if (function == "cosh") {
        expectArguments(function, arguments, 1);
        return fromNative(std::cosh(toNative(arguments[0])));
    }

    if (function == "tanh") {
        expectArguments(function, arguments, 1);
        return fromNative(std::tanh(toNative(arguments[0])));
    }

    if (function == "asinh") {
        expectArguments(function, arguments, 1);
        return fromNative(std::asinh(toNative(arguments[0])));
    }

    if (function == "acosh") {
        expectArguments(function, arguments, 1);
        return fromNative(std::acosh(toNative(arguments[0])));
    }

    if (function == "atanh") {
        expectArguments(function, arguments, 1);
        return fromNative(std::atanh(toNative(arguments[0])));
    }

    /*
     * Complex-specific operations.
     */
    if (function == "abs") {
        expectArguments(function, arguments, 1);

        return core::Value(
            numeric::Real(
                std::abs(toNative(arguments[0]))));
    }

    if (function == "arg") {
        expectArguments(function, arguments, 1);

        return core::Value(
            numeric::Real(
                std::arg(toNative(arguments[0]))));
    }

    if (function == "norm") {
        expectArguments(function, arguments, 1);

        return core::Value(
            numeric::Real(
                std::norm(toNative(arguments[0]))));
    }

    if (function == "conj") {
        expectArguments(function, arguments, 1);
        return fromNative(
            std::conj(toNative(arguments[0])));
    }

    if (function == "real") {
        expectArguments(function, arguments, 1);

        return core::Value(
            numeric::Real(
                toNative(arguments[0]).real()));
    }

    if (function == "imag") {
        expectArguments(function, arguments, 1);

        return core::Value(
            numeric::Real(
                toNative(arguments[0]).imag()));
    }

    throw std::invalid_argument(
        "Function '" + std::string(function) +
        "' is not supported by ComplexDoubleAdapter.");
}

core::Value ComplexDoubleAdapter::resolveConstant(
    std::string_view constant) const
{
    if (constant == "i") {
        return core::Value(
            numeric::Complex(
                numeric::Real(0.0),
                numeric::Real(1.0)));
    }

    if (constant == "pi") {
        return core::Value(
            numeric::Real(std::numbers::pi_v<double>));
    }

    if (constant == "e") {
        return core::Value(
            numeric::Real(std::numbers::e_v<double>));
    }

    throw std::invalid_argument(
        "Constant '" + std::string(constant) +
        "' is not supported by ComplexDoubleAdapter.");
}

std::unique_ptr<MathAdapter> ComplexDoubleAdapter::clone() const
{
    return std::make_unique<ComplexDoubleAdapter>(*this);
}

} // namespace numathap::config
