#pragma once

#include <string>

#include "numathap/config/Capability.hpp"

namespace numathap::config {

class MathEnvironment;

/**
 * @brief Configuration interface for a mathematical execution environment.
 *
 * A Configurator provides the public API used to configure the
 * MathEnvironment associated with a Session.
 *
 * It modifies the Session's MathEnvironment but never performs
 * mathematical computations itself.
 */
class Configurator {
   public:
    /**
     * @brief Constructs a configurator associated with a mathematical
     * execution environment.
     */
    explicit Configurator(MathEnvironment& environment);

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
     * @brief Selects the numeric type.
     *
     * The default numeric type is "double".
     *
     * @param type Numeric type identifier.
     */
    void setNumericType(const std::string& type);

    /**
     * @brief Returns the selected numeric type.
     */
    [[nodiscard]]
    const std::string& numericType() const noexcept;

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

   private:
    MathEnvironment& environment_;
};

}  // namespace numathap::config