/**
 * @file Complex.cpp
 * @brief Implementation of the Complex numeric type.
 */

#include "numathap/numeric/Complex.hpp"

#include <cmath>
#include <stdexcept>

namespace numathap::numeric {

Complex::Complex(const Real& real) noexcept : real_(real) {}

Complex::Complex(const Real& real, const Real& imaginary) noexcept
    : real_(real), imaginary_(imaginary) {}

const Real& Complex::real() const noexcept { return real_; }

const Real& Complex::imaginary() const noexcept { return imaginary_; }

Complex Complex::conjugate() const noexcept {
    return Complex(real_, -imaginary_);
}

Real Complex::norm() const noexcept {
    return (real_ * real_) + (imaginary_ * imaginary_);
}

Real Complex::modulus() const {
    using std::sqrt;

    return Real(sqrt(norm().value()));
}

Complex Complex::operator+(const Complex& other) const noexcept {
    return Complex(real_ + other.real_, imaginary_ + other.imaginary_);
}

Complex Complex::operator-(const Complex& other) const noexcept {
    return Complex(real_ - other.real_, imaginary_ - other.imaginary_);
}

Complex Complex::operator*(const Complex& other) const noexcept {
    const Real real = (real_ * other.real_) -
                      (imaginary_ * other.imaginary_);

    const Real imaginary = (real_ * other.imaginary_) +
                           (imaginary_ * other.real_);

    return Complex(real, imaginary);
}

Complex Complex::operator/(const Complex& other) const {
    const Real denominator = other.norm();

    if (denominator == Real(0.0)) {
        throw std::domain_error("Complex: division by zero.");
    }

    const Real real = ((real_ * other.real_) +
                       (imaginary_ * other.imaginary_)) /
                      denominator;

    const Real imaginary = ((imaginary_ * other.real_) -
                            (real_ * other.imaginary_)) /
                           denominator;

    return Complex(real, imaginary);
}

Complex Complex::operator-() const noexcept {
    return Complex(-real_, -imaginary_);
}

bool Complex::operator==(const Complex& other) const noexcept {
    return real_ == other.real_ && imaginary_ == other.imaginary_;
}

bool Complex::operator!=(const Complex& other) const noexcept {
    return !(*this == other);
}

}  // namespace numathap::numeric
