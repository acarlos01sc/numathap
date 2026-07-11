#pragma once

#include <string>
#include <unordered_set>

#include "numathap/config/Capability.hpp"

namespace numathap::config {

class Configurator;

/**
 * @brief Represents the mathematical execution environment of a Session.
 *
 * A MathEnvironment stores the execution configuration used by the
 * evaluation pipeline, including the selected mathematical library,
 * numeric type and enabled capabilities.
 *
 * Every Session owns exactly one MathEnvironment.
 *
 * The environment is configured exclusively through Configurator and
 * is consulted by runtime components such as the Evaluator,
 * FunctionRegistry and symbolic backends.
 */
class MathEnvironment {
    friend class Configurator;

   public:
    /**
     * @brief Constructs a mathematical environment using the default
     * configuration.
     *
     * Defaults:
     *   - Math library : "cmath"
     *   - Numeric type : "double"
     */
    MathEnvironment() = default;

    /**
     * @brief Returns the selected mathematical library.
     */
    [[nodiscard]]
    const std::string& mathLibrary() const noexcept;

    /**
     * @brief Returns the selected numeric type.
     */
    [[nodiscard]]
    const std::string& numericType() const noexcept;

    /**
     * @brief Checks whether a capability is enabled.
     */
    [[nodiscard]]
    bool hasCapability(Capability capability) const noexcept;

   private:
    /**
     * @brief Selects the mathematical library.
     */
    void setMathLibrary(const std::string& library);

    /**
     * @brief Selects the numeric type.
     */
    void setNumericType(const std::string& type);

    /**
     * @brief Enables a capability.
     */
    void enableCapability(Capability capability);

    /**
     * @brief Disables a capability.
     */
    void disableCapability(Capability capability);

   private:
    std::string math_library_{"cmath"};
    std::string numeric_type_{"double"};

    std::unordered_set<Capability> capabilities_;
};

}  // namespace numathap::config