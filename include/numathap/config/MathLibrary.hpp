/**
 * @file MathLibrary.hpp
 * @brief Defines the supported mathematical library backends.
 */
#pragma once

namespace numathap::config {

/**
 * @brief Identifies the mathematical library used by the environment.
 *
 * This enumeration lists all backends currently supported by the library.
 * The environment uses this identifier to instantiate the corresponding
 * @ref MathAdapter.
 */
enum class MathLibrary {
    /**
     * @brief C++ Standard Library `<cmath>` implementation.
     *
     * This provides standard mathematical functions and constants as defined
     * by the C++ ISO standard.
     */
    CMath
};

}  // namespace numathap::config