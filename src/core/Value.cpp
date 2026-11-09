/**
 * @file Value.cpp
 * @brief Implementation of the numeric Value type.
 */

#include "numathap/core/Value.hpp"

#include <sstream>
#include <stdexcept>
#include <type_traits>

namespace numathap::core {

Value::Value() noexcept : value_(numeric::Real2{}) {}

Value::Value(const numeric::Real2& value) noexcept : value_(value) {}

Value::Value(const numeric::Complex& value) noexcept : value_(value) {}

bool Value::isReal() const noexcept {
    return std::holds_alternative<numeric::Real2>(value_);
}

bool Value::isComplex() const noexcept {
    return std::holds_alternative<numeric::Complex>(value_);
}

const numeric::Real2& Value::real() const {
    return std::get<numeric::Real2>(value_);
}

const numeric::Complex& Value::complex() const {
    return std::get<numeric::Complex>(value_);
}

const Value::Storage& Value::storage() const noexcept { return value_; }

Value Value::operator+(const Value& other) const noexcept {
    return std::visit(
        [](const auto& lhs, const auto& rhs) -> Value {
            using Lhs = std::decay_t<decltype(lhs)>;
            using Rhs = std::decay_t<decltype(rhs)>;

            if constexpr (std::is_same_v<Lhs, numeric::Real2> &&
                          std::is_same_v<Rhs, numeric::Real2>) {
                return Value(lhs + rhs);
            } else {
                return Value(numeric::Complex(lhs) + numeric::Complex(rhs));
            }
        },
        value_, other.value_);
}

Value Value::operator-(const Value& other) const noexcept {
    return std::visit(
        [](const auto& lhs, const auto& rhs) -> Value {
            using Lhs = std::decay_t<decltype(lhs)>;
            using Rhs = std::decay_t<decltype(rhs)>;

            if constexpr (std::is_same_v<Lhs, numeric::Real2> &&
                          std::is_same_v<Rhs, numeric::Real2>) {
                return Value(lhs - rhs);
            } else {
                return Value(numeric::Complex(lhs) - numeric::Complex(rhs));
            }
        },
        value_, other.value_);
}

Value Value::operator*(const Value& other) const noexcept {
    return std::visit(
        [](const auto& lhs, const auto& rhs) -> Value {
            using Lhs = std::decay_t<decltype(lhs)>;
            using Rhs = std::decay_t<decltype(rhs)>;

            if constexpr (std::is_same_v<Lhs, numeric::Real2> &&
                          std::is_same_v<Rhs, numeric::Real2>) {
                return Value(lhs * rhs);
            } else {
                return Value(numeric::Complex(lhs) * numeric::Complex(rhs));
            }
        },
        value_, other.value_);
}

Value Value::operator/(const Value& other) const {
    return std::visit(
        [](const auto& lhs, const auto& rhs) -> Value {
            using Lhs = std::decay_t<decltype(lhs)>;
            using Rhs = std::decay_t<decltype(rhs)>;

            if constexpr (std::is_same_v<Lhs, numeric::Real2> &&
                          std::is_same_v<Rhs, numeric::Real2>) {
                return Value(lhs / rhs);
            } else {
                return Value(numeric::Complex(lhs) / numeric::Complex(rhs));
            }
        },
        value_, other.value_);
}

Value Value::operator+() const noexcept { return *this; }

Value Value::operator-() const noexcept {
    return std::visit([](const auto& value) -> Value { return Value(-value); },
                      value_);
}

bool Value::operator==(const Value& other) const {
    return std::visit(
        [](const auto& lhs, const auto& rhs) -> bool {
            using Lhs = std::decay_t<decltype(lhs)>;
            using Rhs = std::decay_t<decltype(rhs)>;

            if constexpr (std::is_same_v<Lhs, numeric::Real2> &&
                          std::is_same_v<Rhs, numeric::Real2>) {
                return lhs == rhs;
            } else {
                return numeric::Complex(lhs) == numeric::Complex(rhs);
            }
        },
        value_, other.value_);
}

bool Value::operator!=(const Value& other) const { return !(*this == other); }

bool Value::operator<(const Value& other) const {
    if (!isReal() || !other.isReal()) {
        throw std::domain_error(
            "Value: ordering is only defined for real values.");
    }

    return real() < other.real();
}

bool Value::operator<=(const Value& other) const {
    if (!isReal() || !other.isReal()) {
        throw std::domain_error(
            "Value: ordering is only defined for real values.");
    }

    return real() <= other.real();
}

bool Value::operator>(const Value& other) const {
    if (!isReal() || !other.isReal()) {
        throw std::domain_error(
            "Value: ordering is only defined for real values.");
    }

    return real() > other.real();
}

bool Value::operator>=(const Value& other) const {
    if (!isReal() || !other.isReal()) {
        throw std::domain_error(
            "Value: ordering is only defined for real values.");
    }

    return real() >= other.real();
}

Value Value::parse(std::string_view value) {
    return Value(numeric::Real2(value));
}

std::string Value::str() const {
    if (isReal()) {
        return real().toString();
    }

    const auto& value = complex();
    std::ostringstream os;

    os << value.real().toString();

    const auto& imaginary = value.imaginary();

    if (imaginary != numeric::Real2(0)) {
        if (imaginary == numeric::Real2(-1)) {
            os << "-i";
        } else if (imaginary == numeric::Real2(1)) {
            os << "+i";
        } else if (imaginary < numeric::Real2(0)) {
            os << imaginary.toString() << "i";
        } else {
            os << "+" << imaginary.toString() << "i";
        }
    }

    return os.str();
}

std::ostream& operator<<(std::ostream& os, const Value& value) {
    return os << value.str();
}

}  // namespace numathap::core
