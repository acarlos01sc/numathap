#include "numathap/config/MathEnvironment.hpp"

#include <memory>
#include <utility>

#include "numathap/config/CMathDoubleAdapter.hpp"

namespace numathap::config {

MathEnvironment::MathEnvironment()
    : adapter_(std::make_unique<CMathDoubleAdapter>()) {}

MathEnvironment::MathEnvironment(const MathEnvironment& other)
    : math_library_(other.math_library_),
      numeric_type_(other.numeric_type_),
      adapter_(other.adapter_->clone()),
      capabilities_(other.capabilities_),
      integration_algorithm_(other.integration_algorithm_),
      integration_algorithm_config_(other.integration_algorithm_config_) {}

MathEnvironment& MathEnvironment::operator=(const MathEnvironment& other) {
    if (this != &other) {
        math_library_ = other.math_library_;

        numeric_type_ = other.numeric_type_;

        adapter_ = other.adapter_->clone();

        capabilities_ = other.capabilities_;

        integration_algorithm_ = other.integration_algorithm_;

        integration_algorithm_config_ = other.integration_algorithm_config_;
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

backend::integrate::Algorithm MathEnvironment::integrationAlgorithm()
    const noexcept {
    return integration_algorithm_;
}

const backend::integrate::AlgorithmConfig&
MathEnvironment::integrationAlgorithmConfig() const noexcept {
    return integration_algorithm_config_;
}

void MathEnvironment::setIntegrationConfiguration(
    backend::integrate::Algorithm algorithm,
    backend::integrate::AlgorithmConfig config) {
    integration_algorithm_ = algorithm;
    integration_algorithm_config_ = std::move(config);
}

}  // namespace numathap::config