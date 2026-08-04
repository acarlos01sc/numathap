#include "numathap/config/Configurator.hpp"

#include "numathap/backend/integrate/AdaptiveSimpson.hpp"
#include "numathap/backend/integrate/GaussKronrod15.hpp"
#include "numathap/backend/series/Taylor.hpp"
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

void Configurator::apply(
    const backend::integrate::GaussKronrod15Config& config) {
    environment_.setIntegrationConfiguration(
        backend::integrate::Algorithm::GaussKronrod15, config);
}

void Configurator::apply(
    const backend::series::TaylorConfig& config) {
    environment_.setSeriesConfiguration(
        backend::series::SeriesType::Taylor, config);
}

}  // namespace numathap::config