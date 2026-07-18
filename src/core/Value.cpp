#include "numathap/core/Value.hpp"

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
    os << value.real().value();
    return os;
}

}  // namespace numathap::core