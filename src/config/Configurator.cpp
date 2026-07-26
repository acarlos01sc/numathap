#include "numathap/config/Configurator.hpp"

#include "numathap/backend/integrate/AdaptiveSimpson.hpp"
#include "numathap/config/MathEnvironment.hpp"

namespace numathap::config {

Configurator::Configurator(MathEnvironment& environment)
    : environment_(environment) {}

MathLibrary Configurator::mathLibrary() const noexcept {
    return environment_.mathLibrary();
}

NumericType Configurator::numericType() const noexcept {
    return environment_.numericType();
}

void Configurator::enableCapability(Capability capability) {
    environment_.enableCapability(capability);
}

void Configurator::disableCapability(Capability capability) {
    environment_.disableCapability(capability);
}

bool Configurator::hasCapability(Capability capability) const noexcept {
    return environment_.hasCapability(capability);
}

void Configurator::apply(Capability capability) {
    enableCapability(capability);
}

void Configurator::apply(
    const backend::integrate::AdaptiveSimpsonConfig& config) {
    environment_.setIntegrationConfiguration(
        backend::integrate::Algorithm::AdaptiveSimpson, config);
}

}  // namespace numathap::config