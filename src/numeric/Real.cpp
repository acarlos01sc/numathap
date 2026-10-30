#include "numathap/numeric/Real.hpp"

#include <cmath>
#include <cstdlib>
#include <limits>
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

bool Real::isFinite() const noexcept {
    const auto v = value();

    const auto limit = std::numeric_limits<Storage>::max();

    return v >= -limit && v <= limit;
}

Real::Storage Real::epsilon() noexcept {
    return std::numeric_limits<Storage>::epsilon();
}

int Real::sign() const {
    if (!std::isfinite(value_)) {
        throw std::domain_error(
            "Real: sign is undefined for non-finite values.");
    }

    if (value_ > 0.0) {
        return 1;
    }

    if (value_ < 0.0) {
        return -1;
    }

    return 0;
}

bool Real::isInteger() const noexcept {
    return std::isfinite(value_) && std::trunc(value_) == value_;
}

}  // namespace numathap::numeric