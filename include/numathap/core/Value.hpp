#pragma once

#include <ostream>

#include "numathap/numeric/Real.hpp"

namespace numathap::core {

/**
 * @brief Represents a numeric value manipulated by the evaluator.
 *
 * This class is the common numeric type used throughout the evaluation
 * engine. Although it currently stores only a Real value, it is designed
 * to evolve in the future to support additional numeric domains such as
 * Rational, Complex, and arbitrary-precision numbers.
 */
class Value {
   public:
    Value() noexcept = default;
    Value(const numeric::Real&) noexcept;
    explicit Value(numeric::Real::Storage) noexcept;

    [[nodiscard]]
    const numeric::Real& real() const noexcept;

    [[nodiscard]]
    Value operator+(const Value&) const noexcept;

    [[nodiscard]]
    Value operator-(const Value&) const noexcept;

    [[nodiscard]]
    Value operator*(const Value&) const noexcept;

    [[nodiscard]]
    Value operator/(const Value&) const noexcept;

    [[nodiscard]]
    Value operator+() const noexcept;

    [[nodiscard]]
    Value operator-() const noexcept;

    [[nodiscard]]
    bool operator==(const Value&) const noexcept;

    [[nodiscard]]
    bool operator!=(const Value&) const noexcept;

   private:
    numeric::Real value_;
};

std::ostream& operator<<(std::ostream& os, const Value& value);

}  // namespace numathap::core