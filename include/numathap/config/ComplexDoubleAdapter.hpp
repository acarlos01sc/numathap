/**
 * @file ComplexDoubleAdapter.hpp
 * @brief Defines the ComplexDoubleAdapter class for complex mathematical
 *        support using the C++ Standard Library.
 */

#pragma once

#include "numathap/config/MathAdapter.hpp"

namespace numathap::config {

/**
 * @brief Math adapter for complex arithmetic using the C++ Standard Library.
 *
 * This adapter acts as a bridge between the numathap core engine and the
 * standard C++ complex mathematical functions provided by `<complex>`.
 *
 * The adapter operates on @ref numeric::Complex values at the numathap
 * boundary and uses `std::complex<double>` as its native calculation type.
 *
 * For multivalued complex functions, the overload without a branch index
 * represents the principal value. Where mathematically applicable,
 * branch-specific evaluation may be requested through an additional integer
 * argument.
 *
 * @note This class is marked as `final` because it provides a concrete
 *       mathematical backend implementation.
 */
class ComplexDoubleAdapter final : public MathAdapter {

public:
    /**
     * @brief Returns the identifier for this adapter.
     *
     * @return "complex-double"
     */
    [[nodiscard]]
    std::string_view name() const noexcept override;

    /**
     * @brief Evaluates a complex mathematical function.
     *
     * The function is identified by name and receives its arguments through
     * the generic @ref core::Value interface.
     *
     * For multivalued functions, the absence of a branch argument denotes
     * the principal value. Where supported, an additional integer argument
     * selects the requested branch.
     *
     * @param function The name of the mathematical function.
     * @param arguments A span containing the function arguments.
     *
     * @return The result of the function evaluation.
     *
     * @throw std::invalid_argument If the function is not supported or its
     *        arguments are invalid.
     */
    [[nodiscard]]
    core::Value callFunction(
        std::string_view function,
        std::span<const core::Value> arguments) const override;

    /**
     * @brief Resolves a mathematical constant.
     *
     * Constants are returned as complex values, with zero imaginary part
     * when appropriate.
     *
     * @param constant The name of the constant.
     *
     * @return The resolved constant.
     *
     * @throw std::invalid_argument If the constant is not recognized.
     */
    [[nodiscard]]
    core::Value resolveConstant(
        std::string_view constant) const override;

    /**
     * @brief Creates a deep copy of this adapter.
     *
     * @return A unique pointer to the cloned adapter.
     */
    std::unique_ptr<MathAdapter> clone() const override;
};

} // namespace numathap::config