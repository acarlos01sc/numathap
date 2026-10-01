/**
 * @file Complex.hpp
 * @brief Definition of the Complex numeric type.
 *
 * This file provides the abstraction for complex-number arithmetic
 * within the numathap library.
 */
#pragma once

#include "numathap/numeric/Real.hpp"

namespace numathap::numeric {

/**
 * @brief Represents a complex number within the library.
 *
 * A Complex number is represented in Cartesian form as
 *
 *     z = a + bi
 *
 * where both the real and imaginary components are represented by
 * numathap::numeric::Real.
 *
 * The class does not depend on std::complex or directly on a floating-point
 * type. This preserves the numeric abstraction provided by Real and allows
 * the underlying real-number representation to evolve independently.
 *
 * @note The identifiers `i` and `j` are syntactic concepts handled by the
 *       expression evaluator, not by this numeric type.
 */
class Complex {
   public:
    /**
     * @brief Construct a new Complex object initialized to zero.
     *
     * The resulting value is 0 + 0i.
     */
    Complex() noexcept = default;

    /**
     * @brief Construct a purely real complex number.
     *
     * @param real The real component.
     */
    explicit Complex(const Real& real) noexcept;

    /**
     * @brief Construct a complex number from its Cartesian components.
     *
     * @param real The real component.
     * @param imaginary The imaginary component.
     */
    Complex(const Real& real, const Real& imaginary) noexcept;

    /**
     * @brief Return the real component.
     *
     * @return A reference to the real component.
     */
    [[nodiscard]]
    const Real& real() const noexcept;

    /**
     * @brief Return the imaginary component.
     *
     * @return A reference to the imaginary component.
     */
    [[nodiscard]]
    const Real& imaginary() const noexcept;

    /**
     * @brief Return the complex conjugate.
     *
     * For z = a + bi:
     *
     *     conjugate(z) = a - bi.
     *
     * @return The complex conjugate.
     */
    [[nodiscard]]
    Complex conjugate() const noexcept;

    /**
     * @brief Return the squared modulus of the complex number.
     *
     * For z = a + bi:
     *
     *     norm(z) = a² + b².
     *
     * @return The squared modulus.
     */
    [[nodiscard]]
    Real norm() const noexcept;

    /**
     * @brief Return the modulus of the complex number.
     *
     * For z = a + bi:
     *
     *     |z| = sqrt(a² + b²).
     *
     * @return The non-negative modulus.
     */
    [[nodiscard]]
    Real modulus() const;

    /**
     * @brief Add another complex number.
     *
     * @param other The complex number to add.
     * @return The sum.
     */
    [[nodiscard]]
    Complex operator+(const Complex& other) const noexcept;

    /**
     * @brief Subtract another complex number.
     *
     * @param other The complex number to subtract.
     * @return The difference.
     */
    [[nodiscard]]
    Complex operator-(const Complex& other) const noexcept;

    /**
     * @brief Multiply by another complex number.
     *
     * @param other The complex number to multiply by.
     * @return The product.
     */
    [[nodiscard]]
    Complex operator*(const Complex& other) const noexcept;

    /**
     * @brief Divide by another complex number.
     *
     * @param other The divisor.
     * @return The quotient.
     *
     * @throws std::domain_error if the divisor is zero.
     */
    [[nodiscard]]
    Complex operator/(const Complex& other) const;

    /**
     * @brief Negate the complex number.
     *
     * @return The negated complex number.
     */
    [[nodiscard]]
    Complex operator-() const noexcept;

    /**
     * @brief Check equality with another complex number.
     *
     * @param other The complex number to compare with.
     * @return true if both components are equal.
     * @return false otherwise.
     */
    [[nodiscard]]
    bool operator==(const Complex& other) const noexcept;

    /**
     * @brief Check inequality with another complex number.
     *
     * @param other The complex number to compare with.
     * @return true if either component differs.
     * @return false otherwise.
     */
    [[nodiscard]]
    bool operator!=(const Complex& other) const noexcept;

   private:
    /// @brief Real component.
    Real real_{};

    /// @brief Imaginary component.
    Real imaginary_{};
};

}  // namespace numathap::numeric

