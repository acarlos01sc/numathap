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

bool Value::operator<(const Value& other) const noexcept {
    return value_.value() < other.value_.value();
}

bool Value::operator<=(const Value& other) const noexcept {
    return value_.value() <= other.value_.value();
}

bool Value::operator>(const Value& other) const noexcept {
    return value_.value() > other.value_.value();
}

bool Value::operator>=(const Value& other) const noexcept {
    return value_.value() >= other.value_.value();
}

Value Value::parse(std::string_view value) {
    return Value(std::stod(std::string(value)));
}

std::string Value::str() const {
    std::ostringstream os;

    os << std::setprecision(
              std::numeric_limits<numeric::Real::Storage>::max_digits10)
       << value_.value();

    return os.str();
}

std::ostream& operator<<(std::ostream& os, const Value& value) {
    return os << value.str();
}

}  // namespace numathap::core