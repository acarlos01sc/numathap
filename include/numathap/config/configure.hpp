#pragma once

#include "numathap/config/Configurator.hpp"
#include "numathap/config/MathEnvironment.hpp"

namespace numathap::config {

/**
 * @brief Creates a MathEnvironment using the supplied configuration objects.
 *
 * When called without arguments, a default MathEnvironment is returned.
 *
 * Each argument is interpreted by Configurator according to its type.
 */
template <typename... Options>
[[nodiscard]]
MathEnvironment configure(const Options&... options);

}  // namespace numathap::config

#include "numathap/config/configure.tpp"