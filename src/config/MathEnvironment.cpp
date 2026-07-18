#include "numathap/config/MathEnvironment.hpp"

#include <memory>

#include "numathap/config/CMathDoubleAdapter.hpp"

namespace numathap::config {

MathEnvironment::MathEnvironment()
    : adapter_(std::make_unique<CMathDoubleAdapter>()) {}

MathEnvironment::MathEnvironment(const MathEnvironment& other)
    : math_library_(other.math_library_),
      numeric_type_(other.numeric_type_),
      adapter_(other.adapter_->clone()) {}

MathEnvironment& MathEnvironment::operator=(const MathEnvironment& other) {
    if (this != &other) {
        math_library_ = other.math_library_;

        numeric_type_ = other.numeric_type_;

        adapter_ = other.adapter_->clone();
    }

    return *this;
}

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