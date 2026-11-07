/**
 * @file Real2.cpp
 * @brief Implementation of the Real2 numeric type.
 */

#include "numathap/numeric/Real2.hpp"

#include <iomanip>
#include <limits>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <string>

// Conditional includes
#if defined(NUMATHAP_REAL_TYPE_DOUBLE) || \
    defined(NUMATHAP_REAL_TYPE_LONG_DOUBLE)

#include <cmath>

#endif

namespace numathap::numeric {

Real2::Real2(int value) : value_(static_cast<Representation>(value)) {}

Real2::Real2(std::string_view value) {
    if (value.empty()) {
        throw std::invalid_argument(
            "Cannot construct Real2 from an empty string.");
    }

    std::istringstream stream{std::string(value)};
    stream.imbue(std::locale::classic());

    Representation parsed{};

    if (!(stream >> parsed)) {
        throw std::invalid_argument("Invalid numeric representation: " +
                                    std::string(value));
    }

    stream >> std::ws;

    if (!stream.eof()) {
        throw std::invalid_argument("Invalid numeric representation: " +
                                    std::string(value));
    }

    value_ = parsed;
}

Real2::Real2(Representation value) : value_(value) {}

Real2 Real2::operator+(const Real2& other) const {
    Real2 result;
    result.value_ = value_ + other.value_;
    return result;
}

Real2 Real2::operator-(const Real2& other) const {
    Real2 result;
    result.value_ = value_ - other.value_;
    return result;
}

Real2 Real2::operator*(const Real2& other) const {
    Real2 result;
    result.value_ = value_ * other.value_;
    return result;
}

Real2 Real2::operator/(const Real2& other) const {
    Real2 result;
    result.value_ = value_ / other.value_;
    return result;
}

Real2 Real2::operator-() const {
    Real2 result;
    result.value_ = -value_;
    return result;
}

bool Real2::operator==(const Real2& other) const {
    return value_ == other.value_;
}

bool Real2::operator!=(const Real2& other) const {
    return value_ != other.value_;
}

bool Real2::operator<(const Real2& other) const {
    return value_ < other.value_;
}

bool Real2::operator<=(const Real2& other) const {
    return value_ <= other.value_;
}

bool Real2::operator>(const Real2& other) const {
    return value_ > other.value_;
}

bool Real2::operator>=(const Real2& other) const {
    return value_ >= other.value_;
}

Real2 Real2::abs() const {
    Real2 result;

    result.value_ = value_ < Representation{0} ? -value_ : value_;

    if (result.value_ == Representation{0}) {
        result.value_ = Representation{0};
    }

    return result;
}

int Real2::sign() const {
    if (!isFinite()) {
        throw std::domain_error(
            "Cannot determine the sign of a non-finite number.");
    }

    if (value_ > Representation{0}) {
        return 1;
    }

    if (value_ < Representation{0}) {
        return -1;
    }

    return 0;
}

bool Real2::isFinite() const {
    return value_ == value_ &&
           value_ <= std::numeric_limits<Representation>::max() &&
           value_ >= std::numeric_limits<Representation>::lowest();
}

bool Real2::isInteger() const {
    if (!isFinite()) {
        return false;
    }

#if defined(NUMATHAP_REAL_TYPE_DOUBLE) || \
    defined(NUMATHAP_REAL_TYPE_LONG_DOUBLE)

    return std::trunc(value_) == value_;

#elif defined(NUMATHAP_REAL_TYPE_MULTIPRECISION)

#error "Multiprecision isInteger() implementation is pending."

#endif
}

Real2 Real2::sqrt() const {
#if defined(NUMATHAP_REAL_TYPE_DOUBLE) || \
    defined(NUMATHAP_REAL_TYPE_LONG_DOUBLE)

    return Real2(std::sqrt(value_), RawTag{});

#elif defined(NUMATHAP_REAL_TYPE_MULTIPRECISION)

#error "Multiprecision sqrt() implementation is pending."

#endif
}

Real2 Real2::epsilon() {
    Real2 result;
    result.value_ = std::numeric_limits<Representation>::epsilon();
    return result;
}

std::string Real2::toString() const {
    std::ostringstream stream;
    stream.imbue(std::locale::classic());

    stream << std::setprecision(
                  std::numeric_limits<Representation>::max_digits10)
           << value_;

    return stream.str();
}

detail::Real2Representation Real2::nativeValue() const { return value_; }

Real2::Real2(Representation value, RawTag) : value_(value) {}

}  // namespace numathap::numeric