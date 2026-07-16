#include "numathap/config/Configurator.hpp"

#include "numathap/config/MathEnvironment.hpp"

namespace numathap::config {

Configurator::Configurator(MathEnvironment& environment)
    : environment_(environment) {}

void Configurator::setMathLibrary(MathLibrary library) {
    environment_.setMathLibrary(library);
}

MathLibrary Configurator::mathLibrary() const noexcept {
    return environment_.mathLibrary();
}

void Configurator::setNumericType(NumericType type) {
    environment_.setNumericType(type);
}

NumericType Configurator::numericType() const noexcept {
    return environment_.numericType();
}

}  // namespace numathap::config