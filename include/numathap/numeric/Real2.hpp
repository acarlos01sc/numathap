/**
 * @file Real2.hpp
 * @brief Definition of the new real numeric type.
 *
 * This file provides an abstraction over the numeric representation
 * selected at compile time.
 */

#pragma once

#include <string>
#include <string_view>

#include "numathap/numeric/detail/Real2Representation.hpp"

namespace numathap::numeric {

/**
 * @brief Represents a real number within the library.
 *
 * Real2 encapsulates the underlying numeric representation, allowing
 * the library to change its precision without exposing native numeric
 * types through its public interface.
 */
class Real2 {
   public:
    /**
     * @brief Constructs a Real2 initialized to zero.
     */
    Real2() noexcept = default;

    /**
     * @brief Constructs a Real2 from an integer.
     * @param value Initial integer value.
     */
    explicit Real2(int value) noexcept;

    /**
     * @brief Constructs a Real2 from a decimal string.
     *
     * The string is converted directly to the selected representation,
     * preserving as much precision as that representation supports.
     *
     * @param value Decimal representation of the number.
     */
    explicit Real2(std::string_view value);

    /**
     * @brief Constructs a Real2 from the selected native representation.
     * @param value Initial numeric value.
     */
    explicit Real2(detail::Real2Representation value) noexcept;

    /**
     * @brief Adds another real number.
     */
    [[nodiscard]]
    Real2 operator+(const Real2& other) const noexcept;

    /**
     * @brief Subtracts another real number.
     */
    [[nodiscard]]
    Real2 operator-(const Real2& other) const noexcept;

    /**
     * @brief Multiplies by another real number.
     */
    [[nodiscard]]
    Real2 operator*(const Real2& other) const noexcept;

    /**
     * @brief Divides by another real number.
     */
    [[nodiscard]]
    Real2 operator/(const Real2& other) const noexcept;

    /**
     * @brief Returns the negated value.
     */
    [[nodiscard]]
    Real2 operator-() const noexcept;

    /**
     * @brief Returns the sqrt value.
     */
    [[nodiscard]]
    Real2 sqrt() const;

    /**
     * @brief Compares two real numbers for equality.
     */
    [[nodiscard]]
    bool operator==(const Real2& other) const noexcept;

    /**
     * @brief Compares two real numbers for inequality.
     */
    [[nodiscard]]
    bool operator!=(const Real2& other) const noexcept;

    /**
     * @brief Checks whether this value is less than another.
     */
    [[nodiscard]]
    bool operator<(const Real2& other) const noexcept;

    /**
     * @brief Checks whether this value is less than or equal to another.
     */
    [[nodiscard]]
    bool operator<=(const Real2& other) const noexcept;

    /**
     * @brief Checks whether this value is greater than another.
     */
    [[nodiscard]]
    bool operator>(const Real2& other) const noexcept;

    /**
     * @brief Checks whether this value is greater than or equal to another.
     */
    [[nodiscard]]
    bool operator>=(const Real2& other) const noexcept;

    /**
     * @brief Returns the absolute value.
     */
    [[nodiscard]]
    Real2 abs() const noexcept;

    /**
     * @brief Returns the sign of the number.
     *
     * @return -1 for negative values, 0 for zero, and 1 for positive values.
     */
    [[nodiscard]]
    int sign() const;

    /**
     * @brief Checks whether the value is finite.
     */
    [[nodiscard]]
    bool isFinite() const noexcept;

    /**
     * @brief Checks whether the value is an integer.
     */
    [[nodiscard]]
    bool isInteger() const noexcept;

    /**
     * @brief Returns the machine epsilon of the selected representation.
     */
    [[nodiscard]]
    static Real2 epsilon() noexcept;

    /**
     * @brief Converts the value to its decimal string representation.
     */
    [[nodiscard]]
    std::string toString() const;

    /**
     * @brief Returns the underlying numeric representation.
     *
     * @return The value using the numeric type selected at compile time.
     */
    [[nodiscard]]
    detail::Real2Representation nativeValue() const noexcept;

   private:
    using Representation = detail::Real2Representation;

    struct RawTag {};

    Real2(Representation value, RawTag) noexcept;

    Representation value_{0};
};

}  // namespace numathap::numeric