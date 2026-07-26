/**
 * @file Context.hpp
 * @brief Defines the evaluation context for managing symbol definitions and
 * intervals.
 */
#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace numathap::core {

/**
 * @brief Manages environment state, including variable definitions and
 * intervals.
 *
 * The Context class acts as a storage container for symbolic data used during
 * the evaluation of mathematical expressions. It maps identifiers to their
 * definitions or specific numerical intervals.
 */
class Context {
   public:
    /**
     * @brief Represents a numerical interval [lower, upper].
     */
    struct Interval {
        std::string lower;
        std::string upper;
    };

    /**
     * @brief Constructs an empty context.
     */
    Context() = default;

    // ------------------------------------------------------------
    // Variable definitions
    // ------------------------------------------------------------

    /**
     * @brief Associates a symbolic name with a definition (value).
     *
     * If the symbol already exists, its definition is updated.
     *
     * @param symbol The identifier of the variable.
     * @param definition The definition or value associated with the symbol.
     */
    void setValue(const std::string& symbol, const std::string& definition);

    /**
     * @brief Looks up the definition of a symbol.
     *
     * @param symbol The identifier to search for.
     * @return The definition if found, std::nullopt otherwise.
     */
    [[nodiscard]]
    std::optional<std::string> findValue(const std::string& symbol) const;

    /**
     * @brief Resolves a symbolic definition recursively.
     *
     * If the supplied expression corresponds to a symbol defined in this
     * context, its definition is recursively resolved until a terminal
     * expression is reached.
     *
     * Examples:
     * - a -> b -> pi      returns "pi"
     * - x -> sqrt(2)      returns "sqrt(2)"
     * - 2*pi              returns "2*pi"
     *
     * Circular definitions result in an exception.
     *
     * @param expression Expression or symbol to resolve.
     * @return The fully resolved expression.
     */
    [[nodiscard]]
    std::string resolveValue(const std::string& expression) const;

    /**
     * @brief Checks if a value definition exists for the given symbol.
     *
     * @param symbol The identifier to check.
     * @return true if the symbol exists, false otherwise.
     */
    [[nodiscard]]
    bool hasValue(const std::string& symbol) const;

    // ------------------------------------------------------------
    // Interval definitions
    // ------------------------------------------------------------

    /**
     * @brief Associates a symbolic name with an interval [lower, upper].
     *
     * @param symbol The identifier of the symbol.
     * @param lower The lower bound string representation.
     * @param upper The upper bound string representation.
     */
    void setInterval(const std::string& symbol, const std::string& lower,
                     const std::string& upper);

    /**
     * @brief Looks up the interval associated with a symbol.
     *
     * @param symbol The identifier to search for.
     * @return The Interval struct if found, std::nullopt otherwise.
     */
    [[nodiscard]]
    std::optional<Interval> findInterval(const std::string& symbol) const;

    /**
     * @brief Checks if an interval exists for the given symbol.
     *
     * @param symbol The identifier to check.
     * @return true if the interval exists, false otherwise.
     */
    [[nodiscard]]
    bool hasInterval(const std::string& symbol) const;

    // ------------------------------------------------------------

    /**
     * @brief Removes all stored values and intervals from the context.
     */
    void clear();

   private:
    std::unordered_map<std::string, std::string>
        values_;  ///< Map of symbol names to their string definitions.

    std::unordered_map<std::string, Interval>
        intervals_;  ///< Map of symbol names to their associated Intervals.

    [[nodiscard]]
    std::string resolveValue(const std::string& expression,
                             std::unordered_set<std::string>& visited) const;
};

}  // namespace numathap::core