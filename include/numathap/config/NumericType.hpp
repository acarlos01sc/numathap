/**
 * @file NumericType.hpp
 * @brief Defines the numeric representations supported by the library.
 */
#pragma once

namespace numathap::config {

/**
 * @brief Identifies the numeric representation used by the evaluation
 * environment.
 *
 * This enumeration defines the types of numbers that the engine can manipulate.
 * While the current implementation primarily supports @c double, this type is
 * designed to be extensible to accommodate other formats such as rational,
 * complex, or arbitrary-precision numbers in the future.
 */
enum class NumericType {
    /**
     * @brief Standard double-precision floating-point format (IEEE 754).
     */
    Double
};

}  // namespace numathap::config