/**
 * @file Value.hpp
 * @brief Defines the base numeric type used by the evaluation engine.
 */
#pragma once

#include <ostream>
#include <string>
#include <string_view>

#include "numathap/numeric/Real.hpp"

namespace numathap::core {

/**
 * @brief Represents a numeric value manipulated by the evaluator.
 *
 * This class serves as the universal numeric type for the evaluation engine.
 * While it currently encapsulates a @ref numeric::Real value, it is designed
 * as a polymorphic-ready wrapper to support future numeric domains, such as
 * Rational, Complex, or arbitrary-precision types.
 */
class Value {
   public:
    /** @brief Constructs a default Value (typically zero). */
    Value() noexcept = default;

    /** @brief Constructs a Value from a numeric::Real. */
    Value(const numeric::Real&) noexcept;

    /** @brief Constructs a Value directly from the underlying storage type. */
    explicit Value(numeric::Real::Storage) noexcept;

    /**
     * @brief Accesses the underlying Real representation.
     * @return A constant reference to the stored Real value.
     */
    [[nodiscard]]
    const numeric::Real& real() const noexcept;

    /** @name Arithmetic Operators
     *  Arithmetic operations performed on the stored numeric value.
     */
    ///@{
    [[nodiscard]]
    Value operator+(const Value&) const noexcept;
    [[nodiscard]]
    Value operator-(const Value&) const noexcept;
    [[nodiscard]]
    Value operator*(const Value&) const noexcept;
    [[nodiscard]]
    Value operator/(const Value&) const noexcept;

    [[nodiscard]]
    Value operator+() const noexcept;
    [[nodiscard]]
    Value operator-() const noexcept;
    ///@}

    /** @name Comparison Operators */
    ///@{
    [[nodiscard]]
    bool operator==(const Value&) const noexcept;

    [[nodiscard]]
    bool operator!=(const Value&) const noexcept;

    [[nodiscard]]
    bool operator<(const Value&) const noexcept;

    [[nodiscard]]
    bool operator<=(const Value&) const noexcept;

    [[nodiscard]]
    bool operator>(const Value&) const noexcept;

    [[nodiscard]]
    bool operator>=(const Value&) const noexcept;
    ///@}

    /**
     * @brief Parses a textual representation into a Value.
     *
     * @param value Numeric value as text.
     * @return The parsed Value.
     * @throws std::invalid_argument if the text is not a valid number.
     */
    [[nodiscard]]
    static Value parse(std::string_view value);

    /**
     * @brief Returns the canonical textual representation of the value.
     *
     * @return String representation suitable for NumberNode.
     */
    [[nodiscard]]
    std::string str() const;

   private:
    numeric::Real
        value_;  ///< The internal representation of the numeric value.
};

/**
 * @brief Overload of the stream insertion operator for debugging and logging.
 *
 * @param os The output stream.
 * @param value The value to be printed.
 * @return The modified output stream.
 */
std::ostream& operator<<(std::ostream& os, const Value& value);

}  // namespace numathap::core