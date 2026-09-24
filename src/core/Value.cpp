/**
 * @file Value.cpp
 * @brief Implementation of the numeric Value type.
 */

#include "numathap/core/Value.hpp"

#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <type_traits>

namespace numathap::core {

Value::Value() noexcept : value_(numeric::Real{}) {}

Value::Value(const numeric::Real& value) noexcept : value_(value) {}

Value::Value(const numeric::Complex& value) noexcept : value_(value) {}

Value::Value(numeric::Real::Storage value) noexcept : value_(numeric::Real(value)) {}

bool Value::isReal() const noexcept {
    return std::holds_alternative<numeric::Real>(value_);
}

bool Value::isComplex() const noexcept {
    return std::holds_alternative<numeric::Complex>(value_);
}

const numeric::Real& Value::real() const {
    return std::get<numeric::Real>(value_);
}

const numeric::Complex& Value::complex() const {
    return std::get<numeric::Complex>(value_);
}

const Value::Storage& Value::storage() const noexcept {
    return value_;
}

Value Value::operator+(const Value& other) const noexcept {
    return std::visit(
        [](const auto& lhs, const auto& rhs) -> Value {
            using Lhs = std::decay_t<decltype(lhs)>;
            using Rhs = std::decay_t<decltype(rhs)>;

            if constexpr (std::is_same_v<Lhs, numeric::Real> &&
                          std::is_same_v<Rhs, numeric::Real>) {
                return Value(lhs + rhs);
            } else if constexpr (std::is_same_v<Lhs, numeric::Real> &&
                                 std::is_same_v<Rhs, numeric::Complex>) {
                return Value(numeric::Complex(lhs) + rhs);
            } else if constexpr (std::is_same_v<Lhs, numeric::Complex> &&
                                 std::is_same_v<Rhs, numeric::Real>) {
                return Value(lhs + numeric::Complex(rhs));
            } else {
                return Value(lhs + rhs);
            }
        },
        value_, other.value_);
}

Value Value::operator-(const Value& other) const noexcept {
    return std::visit(
        [](const auto& lhs, const auto& rhs) -> Value {
            using Lhs = std::decay_t<decltype(lhs)>;
            using Rhs = std::decay_t<decltype(rhs)>;

            if constexpr (std::is_same_v<Lhs, numeric::Real> &&
                          std::is_same_v<Rhs, numeric::Real>) {
                return Value(lhs - rhs);
            } else if constexpr (std::is_same_v<Lhs, numeric::Real> &&
                                 std::is_same_v<Rhs, numeric::Complex>) {
                return Value(numeric::Complex(lhs) - rhs);
            } else if constexpr (std::is_same_v<Lhs, numeric::Complex> &&
                                 std::is_same_v<Rhs, numeric::Real>) {
                return Value(lhs - numeric::Complex(rhs));
            } else {
                return Value(lhs - rhs);
            }
        },
        value_, other.value_);
}

Value Value::operator*(const Value& other) const noexcept {
    return std::visit(
        [](const auto& lhs, const auto& rhs) -> Value {
            using Lhs = std::decay_t<decltype(lhs)>;
            using Rhs = std::decay_t<decltype(rhs)>;

            if constexpr (std::is_same_v<Lhs, numeric::Real> &&
                          std::is_same_v<Rhs, numeric::Real>) {
                return Value(lhs * rhs);
            } else if constexpr (std::is_same_v<Lhs, numeric::Real> &&
                                 std::is_same_v<Rhs, numeric::Complex>) {
                return Value(numeric::Complex(lhs) * rhs);
            } else if constexpr (std::is_same_v<Lhs, numeric::Complex> &&
                                 std::is_same_v<Rhs, numeric::Real>) {
                return Value(lhs * numeric::Complex(rhs));
            } else {
                return Value(lhs * rhs);
            }
        },
        value_, other.value_);
}

Value Value::operator/(const Value& other) const {
    return std::visit(
        [](const auto& lhs, const auto& rhs) -> Value {
            using Lhs = std::decay_t<decltype(lhs)>;
            using Rhs = std::decay_t<decltype(rhs)>;

            if constexpr (std::is_same_v<Lhs, numeric::Real> &&
                          std::is_same_v<Rhs, numeric::Real>) {
                return Value(lhs / rhs);
            } else if constexpr (std::is_same_v<Lhs, numeric::Real> &&
                                 std::is_same_v<Rhs, numeric::Complex>) {
                return Value(numeric::Complex(lhs) / rhs);
            } else if constexpr (std::is_same_v<Lhs, numeric::Complex> &&
                                 std::is_same_v<Rhs, numeric::Real>) {
                return Value(lhs / numeric::Complex(rhs));
            } else {
                return Value(lhs / rhs);
            }
        },
        value_, other.value_);
}

Value Value::operator+() const noexcept {
    return *this;
}

Value Value::operator-() const noexcept {
    return std::visit(
        [](const auto& value) -> Value {
            return Value(-value);
        },
        value_);
}

bool Value::operator==(const Value& other) const {
    return std::visit(
        [](const auto& lhs, const auto& rhs) -> bool {
            using Lhs = std::decay_t<decltype(lhs)>;
            using Rhs = std::decay_t<decltype(rhs)>;

            if constexpr (std::is_same_v<Lhs, numeric::Real> &&
                          std::is_same_v<Rhs, numeric::Real>) {
                return lhs == rhs;
            } else if constexpr (std::is_same_v<Lhs, numeric::Real> &&
                                 std::is_same_v<Rhs, numeric::Complex>) {
                return numeric::Complex(lhs) == rhs;
            } else if constexpr (std::is_same_v<Lhs, numeric::Complex> &&
                                 std::is_same_v<Rhs, numeric::Real>) {
                return lhs == numeric::Complex(rhs);
            } else {
                return lhs == rhs;
            }
        },
        value_, other.value_);
}

bool Value::operator!=(const Value& other) const {
    return !(*this == other);
}

bool Value::operator<(const Value& other) const {
    if (!isReal() || !other.isReal()) {
        throw std::domain_error(
            "Value: ordering is only defined for real values.");
    }

    return real().value() < other.real().value();
}

bool Value::operator<=(const Value& other) const {
    if (!isReal() || !other.isReal()) {
        throw std::domain_error(
            "Value: ordering is only defined for real values.");
    }

    return real().value() <= other.real().value();
}

bool Value::operator>(const Value& other) const {
    if (!isReal() || !other.isReal()) {
        throw std::domain_error(
            "Value: ordering is only defined for real values.");
    }

    return real().value() > other.real().value();
}

bool Value::operator>=(const Value& other) const {
    if (!isReal() || !other.isReal()) {
        throw std::domain_error(
            "Value: ordering is only defined for real values.");
    }

    return real().value() >= other.real().value();
}

Value Value::parse(std::string_view value) {
    return Value(std::stod(std::string(value)));
}

std::string Value::str() const {
    std::ostringstream os;

    if (isReal()) {
        os << std::setprecision(
                  std::numeric_limits<numeric::Real::Storage>::max_digits10)
           << real().value();

        return os.str();
    }

    const auto& complex = this->complex();

    os << std::setprecision(
              std::numeric_limits<numeric::Real::Storage>::max_digits10)
       << complex.real().value();

    if (complex.imaginary() != numeric::Real(0.0)) {
        if (complex.imaginary() == numeric::Real(-1.0)) {
            os << "-i";
        } else if (complex.imaginary() == numeric::Real(1.0)) {
            os << "+i";
        } else if (complex.imaginary().value() < 0.0) {
            os << complex.imaginary().value() << "i";
        } else {
            os << "+" << complex.imaginary().value() << "i";
        }
    }

    return os.str();
}

std::ostream& operator<<(std::ostream& os, const Value& value) {
    return os << value.str();
}

}  // namespace numathap::core
