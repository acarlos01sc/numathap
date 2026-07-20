/**
 * @file Configurator.hpp
 * @brief Defines the Configurator class for managing environment settings.
 */
#pragma once

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

   private:
    MathEnvironment& environment_; ///< Reference to the associated math environment.
};

}  // namespace numathap::config