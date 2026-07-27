/**
 * @file AdaptiveSimpson.cpp
 * @brief Implements the Adaptive Simpson integration algorithm.
 */

#include "numathap/backend/integrate/AdaptiveSimpson.hpp"

#include <cmath>
#include <stdexcept>
#include <unordered_map>

#include "numathap/backend/Evaluator.hpp"
#include "numathap/math/PreparedAst.hpp"

namespace numathap::backend::integrate {

using core::Value;
using math::PreparedAst;

namespace {

Value evaluateAt(const PreparedAst& prepared,
                 const std::string& variable,
                 const Value& x) {
    return backend::Evaluator::evaluateAt(
        prepared,
        std::unordered_map<std::string, Value>{{variable, x}});
}

Value simpson(const Value& lower,
              const Value& upper,
              const Value& fLower,
              const Value& fMiddle,
              const Value& fUpper) {
    const Value width = upper - lower;

    return (width / Value(6.0)) *
           (fLower + Value(4.0) * fMiddle + fUpper);
}

Value adaptiveSimpson(const PreparedAst& prepared,
                      const std::string& variable,
                      const Value& lower,
                      const Value& upper,
                      const Value& fLower,
                      const Value& fMiddle,
                      const Value& fUpper,
                      const Value& whole,
                      const Value& tolerance,
                      std::size_t depth) {
    const Value middle = (lower + upper) / Value(2.0);

    const Value leftMiddle =
        (lower + middle) / Value(2.0);

    const Value rightMiddle =
        (middle + upper) / Value(2.0);

    const Value fLeftMiddle =
        evaluateAt(prepared, variable, leftMiddle);

    const Value fRightMiddle =
        evaluateAt(prepared, variable, rightMiddle);

    const Value left =
        simpson(
            lower,
            middle,
            fLower,
            fLeftMiddle,
            fMiddle);

    const Value right =
        simpson(
            middle,
            upper,
            fMiddle,
            fRightMiddle,
            fUpper);

    const Value refined = left + right;

    const double error =
        std::abs(
            (refined - whole).real().value());

    const double allowed =
        15.0 * tolerance.real().value();

    if (depth == 0 || error <= allowed) {
        return refined + (refined - whole) / Value(15.0);
    }

    const Value halfTolerance =
        tolerance / Value(2.0);

    return adaptiveSimpson(
               prepared,
               variable,
               lower,
               middle,
               fLower,
               fLeftMiddle,
               fMiddle,
               left,
               halfTolerance,
               depth - 1) +
           adaptiveSimpson(
               prepared,
               variable,
               middle,
               upper,
               fMiddle,
               fRightMiddle,
               fUpper,
               right,
               halfTolerance,
               depth - 1);
}

}  // namespace

Value AdaptiveSimpson::integrate(
    const PreparedAst& prepared,
    const std::string& variable,
    const Value& lower,
    const Value& upper,
    const AdaptiveSimpsonConfig& config) {
    const double tolerance = config.tolerance.real().value();

    if (tolerance <= 0.0) {
        throw std::invalid_argument(
            "AdaptiveSimpson: tolerance must be positive.");
    }

    if (config.maxDepth == 0) {
        throw std::invalid_argument(
            "AdaptiveSimpson: maxDepth must be greater than zero.");
    }

    if (lower == upper) {
        return Value{};
    }

    if (lower.real().value() > upper.real().value()) {
        return -integrate(
            prepared,
            variable,
            upper,
            lower,
            config);
    }

    const Value middle =
        (lower + upper) / Value(2.0);

    const Value fLower =
        evaluateAt(prepared, variable, lower);

    const Value fMiddle =
        evaluateAt(prepared, variable, middle);

    const Value fUpper =
        evaluateAt(prepared, variable, upper);

    const Value whole =
        simpson(
            lower,
            upper,
            fLower,
            fMiddle,
            fUpper);

    return adaptiveSimpson(
        prepared,
        variable,
        lower,
        upper,
        fLower,
        fMiddle,
        fUpper,
        whole,
        config.tolerance,
        config.maxDepth);
}

}  // namespace numathap::backend::integrate