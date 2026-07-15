#pragma once

#include <memory>
#include <string_view>

#include "numathap/improved/PrepareOptions.hpp"
#include "numathap/improved/PreparedExpression.hpp"

namespace numathap::improved {

/**
 * @brief Main entry point of the improved Numathap API.
 *
 * A Session owns the mathematical environment used to prepare
 * and execute expressions.
 *
 * Session objects are lightweight to use but own their internal
 * implementation through the PImpl idiom.
 */
class Session {
public:
    /// Constructs a new mathematical session.
    Session();

    /// Destroys the session.
    ~Session();

    // Non-copyable
    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;

    // Movable
    Session(Session&&) noexcept;
    Session& operator=(Session&&) noexcept;

    /**
     * @brief Parses and prepares a mathematical expression.
     *
     * If no PrepareOptions are supplied, the default preparation
     * pipeline is used.
     *
     * @param expression Mathematical expression.
     * @param options Preparation options.
     * @return PreparedExpression ready for evaluation.
     */
    [[nodiscard]]
    PreparedExpression prepare(
        std::string_view expression,
        const PrepareOptions& options = {}
    ) const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace numathap::improved