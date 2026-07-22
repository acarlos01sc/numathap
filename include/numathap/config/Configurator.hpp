/**
 * @file Configurator.hpp
 * @brief Defines the Configurator class for managing environment settings.
 */
#pragma once

#include "numathap/config/Capability.hpp"
#include "numathap/config/MathLibrary.hpp"
#include "numathap/config/NumericType.hpp"

namespace numathap::config {

class MathEnvironment;

/**
 * @brief Interface for configuring and inspecting the math environment.
 *
 * The Configurator class provides a controlled interface to access and
 * modify the settings within a @ref MathEnvironment. It abstracts
 * the underlying complexity of the environment, offering a clean API
 * for configuration tasks.
 */
class Configurator {
   public:
    /**
     * @brief Constructs a new Configurator instance.
     *
     * @param environment A reference to the @ref MathEnvironment to be managed.
     */
    explicit Configurator(MathEnvironment& environment);

    /**
     * @brief Retrieves the currently configured mathematical library.
     *
     * @return The active @ref MathLibrary enum value.
     */
    [[nodiscard]]
    MathLibrary mathLibrary() const noexcept;

    /**
     * @brief Retrieves the currently configured numeric type representation.
     *
     * @return The active @ref NumericType enum value.
     */
    [[nodiscard]]
    NumericType numericType() const noexcept;

    /**
     * @brief Enables a processing capability.
     *
     * @param capability Capability to enable.
     */
    void enableCapability(Capability capability);

    /**
     * @brief Disables a processing capability.
     *
     * @param capability Capability to disable.
     */
    void disableCapability(Capability capability);

    /**
     * @brief Checks whether a processing capability is enabled.
     *
     * @param capability Capability to query.
     * @return true if the capability is enabled.
     */
    [[nodiscard]]
    bool hasCapability(Capability capability) const noexcept;

    void apply(Capability capability);

    //void apply(IntegrationMethod method);

    //void apply(const Tolerance& tolerance);

    //void apply(const MaxIterations& iterations);

   private:
    MathEnvironment&
        environment_;  ///< Reference to the associated math environment.
};

}  // namespace numathap::config