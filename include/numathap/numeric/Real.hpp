#pragma once

namespace numathap::numeric {

/**
 * @brief Represents a real number.
 *
 * This class encapsulates the underlying floating-point representation
 * used by the library. It provides the basic arithmetic operations and
 * serves as the foundation for the numeric core.
 */
class Real {
   public:
    /**
     * Underlying floating-point representation.
     *
     * Future versions may replace Storage with other floating-point
     * types (e.g. long double or Boost.Multiprecision) without
     * affecting the public API.
     */
    using Storage = double;  // default

    Real() noexcept = default;
    explicit Real(Storage value) noexcept;

    [[nodiscard]]
    Storage value() const noexcept;

    [[nodiscard]]
    Real operator+(const Real&) const noexcept;
    [[nodiscard]]
    Real operator-(const Real&) const noexcept;
    [[nodiscard]]
    Real operator*(const Real&) const noexcept;
    [[nodiscard]]
    Real operator/(const Real&) const noexcept;
    [[nodiscard]]
    Real operator-() const noexcept;
    [[nodiscard]]
    bool operator==(const Real&) const noexcept;
    [[nodiscard]]
    bool operator!=(const Real&) const noexcept;

   private:
    Storage value_{0.0};
};

}  // namespace numathap::numeric