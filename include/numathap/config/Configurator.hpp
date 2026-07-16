#pragma once

#include "numathap/config/MathLibrary.hpp"
#include "numathap/config/NumericType.hpp"

namespace numathap::config {

class MathEnvironment;

class Configurator {
   public:
    explicit Configurator(MathEnvironment& environment);

    [[nodiscard]]
    MathLibrary mathLibrary() const noexcept;

    [[nodiscard]]
    NumericType numericType() const noexcept;

   private:
    MathEnvironment& environment_;
};

}  // namespace numathap::config