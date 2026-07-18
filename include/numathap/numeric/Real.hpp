/**
 * @file Real.hpp
 * @brief Definition of the Real numeric type.
 *
 * This file provides the foundation for floating-point arithmetic within the
 * numathap library.
 */
#pragma once

namespace numathap::numeric {

/**
 * @brief Represents a real number within the library.
 *
 * The Real class acts as an abstraction layer over the underlying
 * floating-point representation. This design allows for seamless transition
 * between different precision types (e.g., float, double, or multiprecision
 * libraries) without breaking the public API.
 */
class Real {
   public:
    /**
     * @brief Underlying floating-point representation type.
     *
     * This alias allows changing the precision of the entire library
     * by modifying a single point.
     */
    using Storage = double;  // default

    /**
     * @brief Construct a new Real object initialized to zero.
     */
    Real() noexcept = default;

    /**
     * @brief Construct a new Real object with a specific value.
     * @param value The initial floating-point value.
     */
    explicit Real(Storage value) noexcept;

    /**
     * @brief Retrieves the raw underlying value.
     * @return The value stored in the Real object.
     */
    [[nodiscard]]
    Storage value() const noexcept;

    /**
     * @brief Adds another Real to this one.
     * @param other The value to add.
     * @return A new Real representing the sum.
     */
    [[nodiscard]]
    Real operator+(const Real&) const noexcept;

    /**
     * @brief Subtracts another Real from this one.
     * @param other The value to subtract.
     * @return A new Real representing the difference.
     */
    [[nodiscard]]
    Real operator-(const Real&) const noexcept;

    /**
     * @brief Multiplies this Real by another.
     * @param other The multiplier.
     * @return A new Real representing the product.
     */
    [[nodiscard]]
    Real operator*(const Real&) const noexcept;

    /**
     * @brief Divides this Real by another.
     * @param other The divisor.
     * @return A new Real representing the quotient.
     */
    [[nodiscard]]
    Real operator/(const Real&) const noexcept;

    /**
     * @brief Negates the Real value.
     * @return A new Real with the negated value.
     */
    [[nodiscard]]
    Real operator-() const noexcept;

    /**
     * @brief Checks for equality between two Real numbers.
     * @param other The Real number to compare with.
     * @return true If values are equal.
     * @return false Otherwise.
     */
    [[nodiscard]]
    bool operator==(const Real&) const noexcept;

    /**
     * @brief Checks for inequality between two Real numbers.
     * @param other The Real number to compare with.
     * @return true If values are not equal.
     * @return false Otherwise.
     */
    [[nodiscard]]
    bool operator!=(const Real&) const noexcept;

   private:
    /// @brief Internal storage for the floating-point value.
    Storage value_{0.0};
};

}  // namespace numathap::numeric