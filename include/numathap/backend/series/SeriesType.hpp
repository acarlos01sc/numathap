/**
 * @file Series.hpp
 * @brief Defines the Series Type available to backend.
 */

#pragma once

namespace numathap::backend::series {

/**
 * @brief Identifies the Series Type.
 *
 * The enumeration identifies the Series Type.
 */
enum class SeriesType {
    /** @brief Taylor Series. */
    Taylor,
};

}  // namespace numathap::backend::series