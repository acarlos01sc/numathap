#pragma once

#include <memory>

#include "numathap/config/MathAdapter.hpp"
#include "numathap/config/MathLibrary.hpp"
#include "numathap/config/NumericType.hpp"

namespace numathap::config {

class Configurator;

class MathEnvironment {
    friend class Configurator;

public:

    MathEnvironment();

    [[nodiscard]]
    MathLibrary mathLibrary() const noexcept;

    [[nodiscard]]
    const MathAdapter& mathAdapter() const noexcept;

    [[nodiscard]]
    NumericType numericType() const noexcept;


private:

    void setMathLibrary(MathLibrary library);

    void setNumericType(NumericType type);

    void rebuildAdapter();


private:

    MathLibrary math_library_{MathLibrary::CMath};

    NumericType numeric_type_{NumericType::Double};

    std::unique_ptr<MathAdapter> adapter_;
};

}