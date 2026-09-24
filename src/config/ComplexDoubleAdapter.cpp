/**
 * @file ComplexDoubleAdapter.cpp
 *
 * @brief Implementation of the ComplexDoubleAdapter class.
 */

#include "numathap/config/ComplexDoubleAdapter.hpp"

#include <complex>
#include <memory>
#include <stdexcept>
#include <string>

namespace numathap::config {

namespace {

using NativeComplex = std::complex<double>;

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

} // namespace

std::string_view ComplexDoubleAdapter::name() const noexcept
{
    return "complex-double";
}

core::Value ComplexDoubleAdapter::callFunction(
    std::string_view function,
    std::span<const core::Value> arguments) const
{
    throw std::invalid_argument(
        "Function '" + std::string(function) +
        "' is not yet implemented by ComplexDoubleAdapter.");
}

core::Value ComplexDoubleAdapter::resolveConstant(
    std::string_view constant) const
{
    throw std::invalid_argument(
        "Constant '" + std::string(constant) +
        "' is not yet implemented by ComplexDoubleAdapter.");
}

std::unique_ptr<MathAdapter> ComplexDoubleAdapter::clone() const
{
    return std::make_unique<ComplexDoubleAdapter>(*this);
}

} // namespace numathap::config
