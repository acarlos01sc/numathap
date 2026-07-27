#include "numathap/core/Value.hpp"

#include <iomanip>
#include <limits>

namespace numathap::core {

Value::Value(const numeric::Real& value) noexcept : value_(value) {}

Value::Value(numeric::Real::Storage value) noexcept : value_(value) {}

const numeric::Real& Value::real() const noexcept { return value_; }

Value Value::operator+(const Value& other) const noexcept {
    return Value(value_ + other.value_);
}

Value Value::operator-(const Value& other) const noexcept {
    return Value(value_ - other.value_);
}

Value Value::operator*(const Value& other) const noexcept {
    return Value(value_ * other.value_);
}

Value Value::operator/(const Value& other) const noexcept {
    return Value(value_ / other.value_);
}

Value Value::operator+() const noexcept { return *this; }

Value Value::operator-() const noexcept { return Value(-value_); }

bool Value::operator==(const Value& other) const noexcept {
    return value_ == other.value_;
}

bool Value::operator!=(const Value& other) const noexcept {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Value& value) {
    os << std::setprecision(
              std::numeric_limits<numeric::Real::Storage>::max_digits10)
       << value.real().value();
    return os;
}

}  // namespace numathap::core