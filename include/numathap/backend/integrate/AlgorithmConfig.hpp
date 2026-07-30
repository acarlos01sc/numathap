/**
 * @file AlgorithmConfig.hpp
 * @brief Defines the type holding algorithm-specific integration
 * configurations.
 */
#pragma once

#include <variant>

#include "numathap/backend/integrate/AdaptiveSimpson.hpp"
#include "numathap/backend/integrate/GaussKronrod15.hpp"

namespace numathap::backend::integrate {

/**
 * @brief Holds the configuration for a supported integration algorithm.
 *
 * The stored type identifies the algorithm-specific configuration.
 *
 * New integration algorithms should add their configuration type to this
 * variant.
 */
using AlgorithmConfig =
    std::variant<AdaptiveSimpsonConfig, GaussKronrod15Config>;

}  // namespace numathap::backend::integrate