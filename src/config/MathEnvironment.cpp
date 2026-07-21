#include "numathap/config/MathEnvironment.hpp"

#include <memory>

#include "numathap/config/CMathDoubleAdapter.hpp"

namespace numathap::config {

MathEnvironment::MathEnvironment()
    : adapter_(std::make_unique<CMathDoubleAdapter>()) {}

MathEnvironment::MathEnvironment(const MathEnvironment& other)
    : math_library_(other.math_library_),
      numeric_type_(other.numeric_type_),
      adapter_(other.adapter_->clone()),
      capabilities_(other.capabilities_) {}

MathEnvironment& MathEnvironment::operator=(const MathEnvironment& other) {
    if (this != &other) {
        math_library_ = other.math_library_;

        numeric_type_ = other.numeric_type_;

        adapter_ = other.adapter_->clone();

        capabilities_ = other.capabilities_;
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

void MathEnvironment::enableCapability(Capability capability) {
    capabilities_.insert(capability);
}

void MathEnvironment::disableCapability(Capability capability) {
    capabilities_.erase(capability);
}

bool MathEnvironment::hasCapability(Capability capability) const noexcept {
    return capabilities_.contains(capability);
}

}  // namespace numathap::config