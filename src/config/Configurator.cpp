#include "numathap/config/Configurator.hpp"

namespace numathap::config {

void Configurator::setMathLibrary(const std::string& library) {
    math_library_ = library;
}

const std::string& Configurator::mathLibrary() const noexcept {
    return math_library_;
}

void Configurator::enableCapability(Capability capability) {
    capabilities_.insert(capability);
}

void Configurator::disableCapability(Capability capability) {
    capabilities_.erase(capability);
}

bool Configurator::hasCapability(Capability capability) const noexcept {
    return capabilities_.contains(capability);
}

MathEnvironment Configurator::build() const {
    return MathEnvironment{
        math_library_,
        capabilities_
    };
}

}  // namespace numathap::config