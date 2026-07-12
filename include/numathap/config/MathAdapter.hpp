#pragma once

#include <span>
#include <string_view>

#include "numathap/core/Value.hpp"

namespace numathap::config {

/**
 * @brief Abstract interface between numathap and a mathematical library.
 *
 * A MathAdapter encapsulates the mathematical functions and constants
 * provided by a specific numerical library (such as the C++ Standard
 * Library or Boost.Math).
 *
 * Numerical evaluation components interact exclusively with this interface,
 * remaining completely independent of any particular mathematical library.
 */
class MathAdapter {
   public:
    virtual ~MathAdapter() = default;

    /**
     * @brief Returns the adapter name.
     *
     * Examples:
     *   - "cmath"
     *   - "boost"
     */
    [[nodiscard]]
    virtual std::string_view name() const noexcept = 0;

    /**
     * @brief Evaluates a mathematical function.
     *
     * @param function Function name.
     * @param arguments Function arguments.
     *
     * @return Function result.
     *
     * @throws std::invalid_argument if the function is unsupported.
     */
    [[nodiscard]]
    virtual core::Value callFunction(
        std::string_view function,
        std::span<const core::Value> arguments) const = 0;

    /**
     * @brief Resolves a mathematical constant.
     *
     * @param constant Constant name.
     *
     * @return Constant value.
     *
     * @throws std::invalid_argument if the constant is unsupported.
     */
    [[nodiscard]]
    virtual core::Value resolveConstant(std::string_view constant) const = 0;
};

}  // namespace numathap::config