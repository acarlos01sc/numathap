/**
 * @file Integrator.cpp
 * @brief Implements the integration dispatcher.
 */

#include "numathap/backend/integrate/Integrator.hpp"

#include <stdexcept>

namespace numathap::backend::integrate {

using config::MathEnvironment;
using core::Context;
using core::Value;
using math::PreparedAst;

Value Integrator::integrate(
    const PreparedAst& prepared,
    const std::string& variable,
    const Context& context) const
{
    return integrate(prepared, variable, context, MathEnvironment{});
}

Value Integrator::integrate(
    const PreparedAst& prepared,
    const std::string& variable,
    const Context& context,
    const MathEnvironment& environment) const
{
    IntegrationInterval interval = resolveInterval(variable, context);

    if (interval.lowerInfinite || interval.upperInfinite)
    {
        return integrateImproper(
            prepared,
            variable,
            interval,
            environment);
    }

    return integrateFinite(
        prepared,
        variable,
        interval,
        environment);
}

Integrator::IntegrationInterval
Integrator::resolveInterval(
    const std::string& variable,
    const Context& context) const
{
    auto interval = context.findInterval(variable);

    if (!interval)
    {
        throw std::runtime_error(
            "Integration interval not found for variable '" +
            variable + "'.");
    }

    IntegrationInterval result;

    //
    // Temporary stub.
    //
    // Future implementation:
    //  - Resolve symbolic limits ("a", "b", "pi", ...)
    //  - Evaluate expressions ("sqrt(2)", "2*pi", ...)
    //  - Detect infinities.
    //

    if (interval->lower == "inf" || interval->lower == "+inf")
    {
        result.lowerInfinite = true;
    }
    else if (interval->lower == "-inf")
    {
        result.lowerInfinite = true;
    }

    if (interval->upper == "inf" || interval->upper == "+inf")
    {
        result.upperInfinite = true;
    }
    else if (interval->upper == "-inf")
    {
        result.upperInfinite = true;
    }

    return result;
}

Value Integrator::integrateFinite(
    const PreparedAst& prepared,
    const std::string& variable,
    const IntegrationInterval& interval,
    const MathEnvironment& environment) const
{
    (void)prepared;
    (void)variable;
    (void)interval;
    (void)environment;

    throw std::runtime_error(
        "Finite interval integration not implemented.");
}

Value Integrator::integrateImproper(
    const PreparedAst& prepared,
    const std::string& variable,
    const IntegrationInterval& interval,
    const MathEnvironment& environment) const
{
    (void)prepared;
    (void)variable;
    (void)interval;
    (void)environment;

    throw std::runtime_error(
        "Improper interval integration not implemented.");
}

} // namespace numathap::backend::integrate