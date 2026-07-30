/**
 * @file GaussKronrod15.cpp
 * @brief Implements the Gauss-Kronrod 15-point integration algorithm.
 */

#include "numathap/backend/integrate/GaussKronrod15.hpp"

#include <stdexcept>

namespace numathap::backend::integrate {

core::Value GaussKronrod15::integrate(
    const math::PreparedAst& /*prepared*/,
    const std::string& /*variable*/,
    const core::Value& /*lower*/,
    const core::Value& /*upper*/,
    const GaussKronrod15Config& /*config*/) {
    throw std::runtime_error(
        "Gauss-Kronrod 15-point integration is not implemented yet.");
}

}  // namespace numathap::backend::integrate