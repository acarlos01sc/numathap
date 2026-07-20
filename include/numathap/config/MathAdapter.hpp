/**
 * @file MathAdapter.hpp
 * @brief Defines the abstract interface for mathematical backend providers.
 */
#pragma once

#include <memory>
#include <span>
#include <string_view>

#include "numathap/core/Value.hpp"

namespace numathap::config {

/**
 * @brief Abstract interface between numathap and a mathematical library.
 *
 * A MathAdapter encapsulates the mathematical functions and constants
 * provided by a specific numerical library (e.g., C++ Standard Library,
 * Boost.Math).
 *
 * Evaluation components interact exclusively with this interface, ensuring
 * that the core engine remains independent of any specific mathematical
 * backend.
 *
 * @note Implementations must be stateless or thread-safe, as they may be
 * accessed concurrently during expression evaluation.
 */
class MathAdapter {
   public:
    /** @brief Virtual destructor for proper polymorphic cleanup. */
    virtual ~MathAdapter() = default;

    /**
     * @brief Creates a deep copy of the adapter instance.
     * @return A unique pointer to the cloned adapter.
     */
    virtual std::unique_ptr<MathAdapter> clone() const = 0;

    /**
     * @brief Returns the unique identifier of the adapter.
     *
     * @return A string view representing the adapter name (e.g., "cmath",
     * "boost").
     */
    [[nodiscard]]
    virtual std::string_view name() const noexcept = 0;

    /**
     * @brief Evaluates a mathematical function.
     *
     * @param function The name of the function to evaluate.
     * @param arguments A span containing the function arguments.
     *
     * @return The result of the function evaluation.
     *
     * @throws std::invalid_argument If the function is not supported or
     * arguments are invalid.
     */
    [[nodiscard]]
    virtual core::Value callFunction(
        std::string_view function,
        std::span<const core::Value> arguments) const = 0;

    /**
     * @brief Resolves a mathematical constant by name.
     *
     * @param constant The name of the constant to resolve (e.g., "pi").
     *
     * @return The constant's value.
     *
     * @throws std::invalid_argument If the constant is not recognized.
     */
    [[nodiscard]]
    virtual core::Value resolveConstant(std::string_view constant) const = 0;
};

}  // namespace numathap::config