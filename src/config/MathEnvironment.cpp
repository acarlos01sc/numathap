#include "numathap/config/MathEnvironment.hpp"

namespace numathap::config {

const std::string& MathEnvironment::mathLibrary() const noexcept {
    return math_library_;
}

const std::string& MathEnvironment::numericType() const noexcept {
    return numeric_type_;
}

bool MathEnvironment::hasCapability(
    Capability capability) const noexcept {
    return capabilities_.contains(capability);
}

void MathEnvironment::setMathLibrary(
    const std::string& library) {
    math_library_ = library;
}

void MathEnvironment::setNumericType(
    const std::string& type) {
    numeric_type_ = type;
}

void MathEnvironment::enableCapability(
    Capability capability) {
    capabilities_.insert(capability);
}

void MathEnvironment::disableCapability(
    Capability capability) {
    capabilities_.erase(capability);
}

}  // namespace numathap::config