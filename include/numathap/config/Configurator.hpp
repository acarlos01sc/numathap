#pragma once

#include <string>
#include <unordered_set>

#include "numathap/config/Capability.hpp"
#include "numathap/config/MathEnvironment.hpp"

namespace numathap::config {

/**
 * @brief Stores the mathematical execution configuration.
 *
 * The Configurator records the execution options selected by the user.
 * It does not execute mathematical operations.
 *
 * Future versions will use this configuration to build the corresponding
 * MathEnvironment consumed by the execution pipeline.
 */
class Configurator {
   public:
    Configurator() = default;

    /**
     * @brief Selects the mathematical library.
     *
     * The default library is "cmath".
     *
     * @param library Library identifier.
     */
    void setMathLibrary(const std::string& library);

    /**
     * @brief Returns the selected mathematical library.
     */
    [[nodiscard]]
    const std::string& mathLibrary() const noexcept;

    /**
     * @brief Enables a capability.
     */
    void enableCapability(Capability capability);

    /**
     * @brief Disables a capability.
     */
    void disableCapability(Capability capability);

    /**
     * @brief Checks whether a capability is enabled.
     */
    [[nodiscard]]
    bool hasCapability(Capability capability) const noexcept;

    [[nodiscard]]
    MathEnvironment build() const;

   private:
    std::string math_library_{"cmath"};
    std::unordered_set<Capability> capabilities_;
};

}  // namespace numathap::config