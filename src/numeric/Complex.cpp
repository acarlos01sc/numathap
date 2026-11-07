/**
 * @file Complex.cpp
 * @brief Implementation of the Complex numeric type.
 */

#include "numathap/numeric/Complex.hpp"

#include <stdexcept>

namespace numathap::numeric {

Complex::Complex(const Real2& real) noexcept
    : real_(real) {}

Complex::Complex(const Real2& real, const Real2& imaginary) noexcept
    : real_(real), imaginary_(imaginary) {}

const Real2& Complex::real() const noexcept {
    return real_;
}

const Real2& Complex::imaginary() const noexcept {
    return imaginary_;
}

Complex Complex::conjugate() const noexcept {
    return Complex(real_, -imaginary_);
}

Real2 Complex::norm() const noexcept {
    return (real_ * real_) + (imaginary_ * imaginary_);
}

Real2 Complex::modulus() const {
    return norm().sqrt();
}

Complex Complex::operator+(const Complex& other) const noexcept {
    return Complex(
        real_ + other.real_,
        imaginary_ + other.imaginary_
    );
}

Complex Complex::operator-(const Complex& other) const noexcept {
    return Complex(
        real_ - other.real_,
        imaginary_ - other.imaginary_
    );
}

Complex Complex::operator*(const Complex& other) const noexcept {
    const Real2 real =
        (real_ * other.real_) - (imaginary_ * other.imaginary_);

    const Real2 imaginary =
        (real_ * other.imaginary_) + (imaginary_ * other.real_);

    return Complex(real, imaginary);
}

Complex Complex::operator/(const Complex& other) const {
    const Real2 denominator = other.norm();

    if (denominator == Real2(0)) {
        throw std::domain_error("Complex: division by zero.");
    }

    const Real2 real =
        ((real_ * other.real_) + (imaginary_ * other.imaginary_))
        / denominator;

    const Real2 imaginary =
        ((imaginary_ * other.real_) - (real_ * other.imaginary_))
        / denominator;

    return Complex(real, imaginary);
}

Complex Complex::operator-() const noexcept {
    return Complex(-real_, -imaginary_);
}

bool Complex::operator==(const Complex& other) const noexcept {
    return real_ == other.real_ &&
           imaginary_ == other.imaginary_;
}

bool Complex::operator!=(const Complex& other) const noexcept {
    return !(*this == other);
}

} // namespace numathap::numeric