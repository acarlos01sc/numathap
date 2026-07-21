#include <cassert>
#include <iostream>

#include "numathap/config/Capability.hpp"
#include "numathap/config/MathEnvironment.hpp"

int main()
{
    using namespace numathap::config;

    MathEnvironment environment;


    //
    // Default environment has no enabled capabilities.
    //
    assert(
        !environment.hasCapability(
            Capability::Simplify));


    //
    // Enable capability.
    //
    environment.enableCapability(
        Capability::Simplify);

    assert(
        environment.hasCapability(
            Capability::Simplify));


    //
    // Disable capability.
    //
    environment.disableCapability(
        Capability::Simplify);

    assert(
        !environment.hasCapability(
            Capability::Simplify));


    //
    // Copy must preserve capabilities.
    //
    environment.enableCapability(
        Capability::Simplify);

    MathEnvironment copy(environment);

    assert(
        copy.hasCapability(
            Capability::Simplify));


    //
    // Assignment must preserve capabilities.
    //
    MathEnvironment assigned;

    assigned = environment;

    assert(
        assigned.hasCapability(
            Capability::Simplify));


    std::cout
        << "All MathEnvironment capability tests passed.\n";

    return 0;
}