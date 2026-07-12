#pragma once

#include <unordered_set>

#include "numathap/config/Capability.hpp"
#include "numathap/config/MathLibrary.hpp"
#include "numathap/config/NumericType.hpp"

namespace numathap::config {

class Configurator;

/**
 * @brief Represents the mathematical execution environment of a Session.
 *
 * A MathEnvironment stores the execution configuration used by the
 * evaluation pipeline, including the selected mathematical library,
 * numeric type and enabled capabilities.
 *
 * The environment is configured exclusively through Configurator.
 */
class MathEnvironment {
    friend class Configurator;

   public:
    /**
     * @brief Constructs a mathematical environment using default values.
     *
     * Defaults:
     *   - Math library : MathLibrary::CMath
     *   - Numeric type : NumericType::Double
     */
    MathEnvironment() = default;

    /**
     * @brief Returns the selected mathematical library.
     */
    [[nodiscard]]
    MathLibrary mathLibrary() const noexcept;

    /**
     * @brief Returns the selected numeric type.
     */
    [[nodiscard]]
    NumericType numericType() const noexcept;

    /**
     * @brief Checks whether a capability is enabled.
     */
    [[nodiscard]]
    bool hasCapability(Capability capability) const noexcept;

   private:
    void setMathLibrary(MathLibrary library);

    void setNumericType(NumericType type);

    void enableCapability(Capability capability);

    void disableCapability(Capability capability);

   private:
    MathLibrary math_library_{MathLibrary::CMath};

    NumericType numeric_type_{NumericType::Double};

    std::unordered_set<Capability> capabilities_;
};

}  // namespace numathap::config