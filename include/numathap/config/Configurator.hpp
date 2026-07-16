#pragma once

#include "numathap/config/Capability.hpp"
#include "numathap/config/MathLibrary.hpp"
#include "numathap/config/NumericType.hpp"

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
     * @param library Mathematical library identifier.
     */
    void setMathLibrary(MathLibrary library);

    /**
     * @brief Returns the selected mathematical library.
     */
    [[nodiscard]]
    MathLibrary mathLibrary() const noexcept;

    /**
     * @brief Selects the numeric type.
     *
     * @param type Numeric type identifier.
     */
    void setNumericType(NumericType type);

    /**
     * @brief Returns the selected numeric type.
     */
    [[nodiscard]]
    NumericType numericType() const noexcept;

   private:
    MathEnvironment& environment_;
};

}  // namespace numathap::config