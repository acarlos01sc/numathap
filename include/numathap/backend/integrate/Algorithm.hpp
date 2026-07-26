/**
 * @file Algorithm.hpp
 * @brief Defines the integration algorithms available to the numeric backend.
 */

#pragma once

namespace numathap::backend::integrate {

/**
 * @brief Identifies the numerical integration algorithm.
 *
 * The enumeration identifies an algorithm independently from its
 * algorithm-specific configuration.
 */
enum class Algorithm {
    AdaptiveSimpson
};

}  // namespace numathap::backend::integrate