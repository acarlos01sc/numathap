#pragma once

#include <string>
#include <unordered_set>

#include "numathap/config/Capability.hpp"

namespace numathap::config {

/**
 * @brief Represents the resolved mathematical execution environment.
 *
 * A MathEnvironment is an immutable snapshot of the execution
 * configuration produced by a Configurator.
 *
 * Runtime components receive a MathEnvironment instead of consulting
 * the Configurator directly.
 *
 * Future versions will also contain the resolved Math Backend,
 * Function Registry and additional execution resources.
 */
class MathEnvironment {
   public:
    MathEnvironment(
        std::string math_library,
        std::unordered_set<Capability> capabilities);

    /**
     * @brief Returns the selected mathematical library.
     */
    [[nodiscard]]
    const std::string& mathLibrary() const noexcept;

    /**
     * @brief Checks whether a capability is enabled.
     */
    [[nodiscard]]
    bool hasCapability(Capability capability) const noexcept;

   private:
    std::string math_library_;
    std::unordered_set<Capability> capabilities_;
};

}  // namespace numathap::config