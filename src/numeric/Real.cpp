#include "numathap/numeric/Real.hpp"

#include <cstdlib>
#include <stdexcept>

namespace numathap::numeric {

Real::Real(Storage value) noexcept : value_(value) {}

Real::Storage Real::value() const noexcept { return value_; }

Real::Real(const char* value) {
    if (value == nullptr) {
        throw std::invalid_argument("Real: null string.");
    }

    char* end = nullptr;
    const double parsed = std::strtod(value, &end);

    if (end == value || *end != '\0') {
        throw std::invalid_argument("Real: invalid numeric value.");
    }

    value_ = parsed;
}

Real Real::operator+(const Real& other) const noexcept {
    return Real(value_ + other.value_);
}

Real Real::operator-(const Real& other) const noexcept {
    return Real(value_ - other.value_);
}

Real Real::operator-() const noexcept { return Real(-value_); }

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