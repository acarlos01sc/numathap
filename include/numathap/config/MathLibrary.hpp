#pragma once

namespace numathap::config {

/**
 * @brief Identifies the mathematical library used by the environment.
 *
 * Only libraries effectively supported by numathap should be listed here.
 */
enum class MathLibrary {
    CMath
};

}  // namespace numathap::config