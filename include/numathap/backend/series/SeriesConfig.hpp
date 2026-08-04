/**
 * @file SeriesConfig.hpp
 * @brief Defines the type holding series
 * configurations.
 */
#pragma once

#include <variant>

#include "numathap/backend/series/Taylor.hpp"

namespace numathap::backend::series {

/**
 * @brief Holds the configuration for a supported series type.
 *
 * The stored type identifies the series-specific configuration.
 *
 * New series types should add their configuration type to this variant.
 */
using SeriesConfig = std::variant<TaylorConfig>;

}  // namespace numathap::backend::series