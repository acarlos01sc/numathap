#pragma once

#include "numathap/config/MathAdapter.hpp"

namespace numathap::config {

/**
 * @brief Math adapter for the C++ Standard Library (cmath).
 *
 * This adapter exposes the mathematical functions and constants
 * provided by <cmath> through the generic MathAdapter interface.
 */
class CMathDoubleAdapter final : public MathAdapter {
   public:
    [[nodiscard]]
    std::string_view name() const noexcept override;

    [[nodiscard]]
    core::Value callFunction(
        std::string_view function,
        std::span<const core::Value> arguments) const override;

    [[nodiscard]]
    core::Value resolveConstant(
        std::string_view constant) const override;
};

}  // namespace numathap::config