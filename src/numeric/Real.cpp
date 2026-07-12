#include "numathap/numeric/Real.hpp"

namespace numathap::numeric {

Real::Real(Storage value) noexcept : value_(value) {}

Real::Storage Real::value() const noexcept { return value_; }

Real Real::operator+(const Real& other) const noexcept {
    return Real(value_ + other.value_);
}

Real Real::operator-(const Real& other) const noexcept {
    return Real(value_ - other.value_);
}

Real Real::operator-() const noexcept {
    return Real(-value_);
}

Real Real::operator*(const Real& other) const noexcept {
    return Real(value_ * other.value_);
}

Real Real::operator/(const Real& other) const noexcept {
    return Real(value_ / other.value_);
}

bool Real::operator==(const Real& other) const noexcept {
    return value_ == other.value_;
}

bool Real::operator!=(const Real& other) const noexcept {
    return !(*this == other);
}

}  // namespace numathap::numeric