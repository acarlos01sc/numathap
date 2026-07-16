#pragma once

#include <memory>

#include "numathap/config/MathAdapter.hpp"
#include "numathap/config/MathLibrary.hpp"
#include "numathap/config/NumericType.hpp"

namespace numathap::config {

class MathEnvironment {
   public:
    MathEnvironment();

    [[nodiscard]]
    MathLibrary mathLibrary() const noexcept;

    [[nodiscard]]
    const MathAdapter& mathAdapter() const noexcept;

    [[nodiscard]]
    NumericType numericType() const noexcept;

   private:
    const MathLibrary math_library_{MathLibrary::CMath};

    const NumericType numeric_type_{NumericType::Double};

    std::unique_ptr<MathAdapter> adapter_;
};

}  // namespace numathap::config