#include "numathap/config/Configurator.hpp"

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

}  // namespace numathap::config