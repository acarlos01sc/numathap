#include "numathap/config/MathEnvironment.hpp"
#include <utility>

namespace numathap::config {

MathEnvironment::MathEnvironment(
    std::string math_library,
    std::unordered_set<Capability> capabilities)
    : math_library_(std::move(math_library)),
      capabilities_(std::move(capabilities)) {}

const std::string& MathEnvironment::mathLibrary() const noexcept {
    return math_library_;
}

bool MathEnvironment::hasCapability(
    Capability capability) const noexcept {
    return capabilities_.contains(capability);
}

}  // namespace numathap::config