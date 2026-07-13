#include "numathap/config/MathEnvironment.hpp"

#include <memory>
//#include <stdexcept>

#include "numathap/config/CMathAdapter.hpp"

namespace numathap::config {

MathLibrary MathEnvironment::mathLibrary() const noexcept {
    return math_library_;
}

MathEnvironment::MathEnvironment()
{
    rebuildAdapter();
}

const MathAdapter&
MathEnvironment::mathAdapter() const noexcept
{
    return *adapter_;
}

void MathEnvironment::rebuildAdapter()
{
    switch (math_library_)
    {
        case MathLibrary::CMath:
            adapter_ = std::make_unique<CMathAdapter>();
            break;
    }
}

//std::unique_ptr<MathAdapter> MathEnvironment::createMathAdapter() const
//{
//    switch (math_library_) {
//        case MathLibrary::CMath:
//            return std::make_unique<CMathAdapter>();

        // Futuras bibliotecas:
        // case MathLibrary::Boost:
        //     return std::make_unique<BoostMathAdapter>();
//    }
//
//    throw std::logic_error("Unsupported math library.");
//}

NumericType MathEnvironment::numericType() const noexcept {
    return numeric_type_;
}

bool MathEnvironment::hasCapability(
    Capability capability) const noexcept {
    return capabilities_.contains(capability);
}

void MathEnvironment::setMathLibrary(
    MathLibrary library) {
    math_library_ = library;
    rebuildAdapter();
}

void MathEnvironment::setNumericType(
    NumericType type) {
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