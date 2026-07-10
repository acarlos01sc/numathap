#pragma once

#include <string>

#include "numathap/config/Configurator.hpp"
#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"

namespace numathap {

/**
 * @brief Main entry point of the numathap library.
 *
 * A Session represents an isolated mathematical execution environment.
 *
 * It owns the execution configuration and provides access to the public
 * numerical and symbolic services offered by the library.
 *
 * Users typically interact only with Session rather than constructing
 * internal components directly.
 */
class Session {
   public:
    Session();

    /**
     * @brief Evaluates a mathematical expression.
     *
     * @param expression Mathematical expression.
     * @param context Variable values.
     *
     * @return Computed value.
     */
    [[nodiscard]]
    core::Value evaluate(
        const std::string& expression,
        const core::Context& context = {}
    ) const;

    /**
     * @brief Returns the configuration interface.
     */
    config::Configurator& configurator() noexcept;

    /**
     * @brief Returns the configuration interface.
     */
    [[nodiscard]]
    const config::Configurator& configurator() const noexcept;

   private:
    config::Configurator configurator_;
};

} // namespace numathap