#include "numathap/config/MathEnvironment.hpp"

#include <memory>

#include "numathap/config/CMathAdapter.hpp"

namespace numathap::config {

MathEnvironment::MathEnvironment()
    : adapter_(std::make_unique<CMathAdapter>()) {}

MathLibrary MathEnvironment::mathLibrary() const noexcept {
    return math_library_;
}

const MathAdapter& MathEnvironment::mathAdapter() const noexcept {
    return *adapter_;
}

NumericType MathEnvironment::numericType() const noexcept {
    return numeric_type_;
}

}  // namespace numathap::config