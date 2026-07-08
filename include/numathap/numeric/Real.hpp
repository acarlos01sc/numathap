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
    using Storage = double;

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
    bool operator==(const Real&) const noexcept;
    [[nodiscard]]
    bool operator!=(const Real&) const noexcept;

   private:
    Storage value_{0.0};
};

}  // namespace numathap::numeric