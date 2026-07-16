#include "numathap/config/MathEnvironment.hpp"

#include <memory>

#include "numathap/config/CMathAdapter.hpp"

namespace numathap::config {

MathEnvironment::MathEnvironment() { rebuildAdapter(); }

MathLibrary MathEnvironment::mathLibrary() const noexcept {
    return math_library_;
}

const MathAdapter& MathEnvironment::mathAdapter() const noexcept {
    return *adapter_;
}

NumericType MathEnvironment::numericType() const noexcept {
    return numeric_type_;
}


void MathEnvironment::setMathLibrary(MathLibrary library) {
    math_library_ = library;
    rebuildAdapter();
}

void MathEnvironment::setNumericType(NumericType type) { numeric_type_ = type; }


void MathEnvironment::rebuildAdapter() {
    switch (math_library_) {
        case MathLibrary::CMath:
            adapter_ = std::make_unique<CMathAdapter>();
            break;
    }
}

}  // namespace numathap::config