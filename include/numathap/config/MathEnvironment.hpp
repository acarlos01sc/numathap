#pragma once

#include <memory>

#include "numathap/config/MathAdapter.hpp"
#include "numathap/config/MathLibrary.hpp"
#include "numathap/config/NumericType.hpp"

namespace numathap::config {

class MathEnvironment {
   public:
    MathEnvironment();

    MathEnvironment(const MathEnvironment&);
    MathEnvironment& operator=(const MathEnvironment&);

    MathEnvironment(MathEnvironment&&) noexcept = default;
    MathEnvironment& operator=(MathEnvironment&&) noexcept = default;

    ~MathEnvironment() = default;

    [[nodiscard]]
    MathLibrary mathLibrary() const noexcept;

    [[nodiscard]]
    const MathAdapter& mathAdapter() const noexcept;

    [[nodiscard]]
    NumericType numericType() const noexcept;

   private:
    MathLibrary math_library_{MathLibrary::CMath};

    NumericType numeric_type_{NumericType::Double};

    std::unique_ptr<MathAdapter> adapter_;
};

}  // namespace numathap::config