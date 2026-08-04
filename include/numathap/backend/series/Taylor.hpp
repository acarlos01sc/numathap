/**
 * @file Taylor.hpp
 * @brief Defines the configuration and implementation interface for the
 *        Taylor Series.
 */

#pragma once

#include <cstddef>
#include <string>

#include "numathap/core/Value.hpp"

namespace numathap::math {
class PreparedAst;
}

namespace numathap::backend::series {

/**
 * @brief Configuration parameters for the Taylor Series.
 *
 */
struct TaylorConfig {
    std::size_t order = 10;
};

/**
 * @brief Implements the Taylor Series.
 *
 */
class Taylor {
   public:
    /**
     * @brief Obtain the Taylor Series expression.
     *
     * @param prepared Prepared mathematical expression.
     * @param variable Expansion variable.
     * @param center Expansion center.
     * @param config Taylor Series configuration.
     *
     * @return Prepared expression containing the Taylor expansion.
     */
    [[nodiscard]]
    static math::PreparedAst series(const math::PreparedAst& prepared,
                                    const std::string& variable,
                                    const core::Value& center,
                                    const TaylorConfig& config);
};

}  // namespace numathap::backend::series