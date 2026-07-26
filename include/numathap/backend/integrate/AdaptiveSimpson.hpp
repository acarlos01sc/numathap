/**
 * @file AdaptiveSimpson.hpp
 * @brief Defines the configuration for the Adaptive Simpson integration
 * algorithm.
 */

#pragma once

#include <cstddef>

#include "numathap/core/Value.hpp"

namespace numathap::backend::integrate {

/**
 * @brief Configuration parameters for the Adaptive Simpson algorithm.
 */
struct AdaptiveSimpsonConfig {
    core::Value tolerance = core::Value(1e-10);
    std::size_t maxDepth = 20;
};

}  // namespace numathap::backend::integrate