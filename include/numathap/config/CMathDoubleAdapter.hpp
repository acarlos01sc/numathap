/**
 * @file CMathDoubleAdapter.hpp
 * @brief Defines the CMathDoubleAdapter class for Standard Library mathematical support.
 */
#pragma once

#include "numathap/config/MathAdapter.hpp"

namespace numathap::config {

/**
 * @brief Math adapter for the C++ Standard Library (`<cmath>`).
 *
 * This adapter acts as a bridge between the @ref numathap core engine and 
 * the standard C++ mathematical functions. It implements the @ref MathAdapter 
 * interface to provide support for standard operations and constants, 
 * using `double` as the primary numeric type.
 *
 * @note This class is marked as `final` as it provides a specific backend 
 * implementation for the evaluation engine.
 */
class CMathDoubleAdapter final : public MathAdapter {
   public:
    /** 
     * @brief Returns the identifier for this adapter.
     * @return "cmath"
     */
    [[nodiscard]]
    std::string_view name() const noexcept override;

    /**
     * @brief Evaluates a function using the C++ standard library implementations.
     * 
     * @param function The name of the mathematical function (e.g., "sin", "exp").
     * @param arguments A span of arguments to pass to the function.
     * @return The evaluated @ref core::Value.
     * @throw std::invalid_argument If the function is not supported.
     */
    [[nodiscard]]
    core::Value callFunction(
        std::string_view function,
        std::span<const core::Value> arguments) const override;

    /**
     * @brief Resolves mathematical constants using standard library definitions.
     * 
     * @param constant The name of the constant (e.g., "pi", "e").
     * @return The resolved @ref core::Value.
     * @throw std::invalid_argument If the constant is not recognized.
     */
    [[nodiscard]]
    core::Value resolveConstant(
        std::string_view constant) const override;

     /**
     * @brief Creates a deep copy of this adapter.
     * @return A unique pointer to the cloned adapter.
     */
    std::unique_ptr<MathAdapter> clone() const override;
};

}  // namespace numathap::config