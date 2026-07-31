/**
 * @file GaussKronrod15.cpp
 * @brief Implements the Gauss-Kronrod 15-point integration algorithm.
 */

#include "numathap/backend/integrate/GaussKronrod15.hpp"

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "numathap/backend/Evaluator.hpp"
#include "numathap/math/PreparedAst.hpp"
#include "numathap/numeric/Real.hpp"

namespace numathap::backend::integrate {

using core::Value;
using math::PreparedAst;
using Storage = numeric::Real::Storage;

namespace {

/**
 * @brief Gauss-Kronrod 15-point quadrature constants.
 *
 * The Kronrod 15-point rule embeds the 7-point Gauss rule.
 *
 * Constants are represented as decimal strings and converted through
 * numeric::Real before being stored as Value.
 */

// Positive abscissae of the Kronrod rule.
const Value x1{numeric::Real("0.991455371120812639206854697526329")};
const Value x2{numeric::Real("0.949107912342758524526189684047851")};
const Value x3{numeric::Real("0.864864423359769072789712788640926")};
const Value x4{numeric::Real("0.741531185599394439863864773280788")};
const Value x5{numeric::Real("0.586087235467691130294144838258730")};
const Value x6{numeric::Real("0.405845151377397166906606412076961")};
const Value x7{numeric::Real("0.207784955007898467600689403773245")};

// Kronrod weights.
const Value wk1{numeric::Real("0.022935322010529224963732008058970")};
const Value wk2{numeric::Real("0.063092092629978553290700663189204")};
const Value wk3{numeric::Real("0.104790010322250183839876322541518")};
const Value wk4{numeric::Real("0.140653259715525918745189590510238")};
const Value wk5{numeric::Real("0.169004726639267902826583426359434")};
const Value wk6{numeric::Real("0.190350578064785409913256402421014")};
const Value wk7{numeric::Real("0.204432940075298892414161999234649")};
const Value wk8{numeric::Real("0.209482141084727828012999174891714")};

// Gauss weights.
const Value wg1{numeric::Real("0.129484966168869693270611432684755")};
const Value wg2{numeric::Real("0.279705391489276667901467771423780")};
const Value wg3{numeric::Real("0.381830050505118944950369775488975")};
const Value wg4{numeric::Real("0.417959183673469387755102040810034")};

Value evaluateAt(const PreparedAst& prepared, const std::string& variable,
                 const Value& x) {
    return backend::Evaluator::evaluateAt(
        prepared, std::unordered_map<std::string, Value>{{variable, x}});
}

struct QuadratureResult {
    Value kronrod{};
    Value gauss{};
    Storage error{};
};

/**
 * @brief Evaluates the embedded Gauss 7 / Kronrod 15 rules on one interval.
 */
QuadratureResult quadrature(const PreparedAst& prepared,
                            const std::string& variable, const Value& lower,
                            const Value& upper, std::size_t& evaluations,
                            std::size_t maxEvaluations) {
    const Value center = (lower + upper) / Value(2.0);

    const Value halfWidth = (upper - lower) / Value(2.0);

    const Value x[] = {x1, x2, x3, x4, x5, x6, x7};

    const Value wk[] = {wk1, wk2, wk3, wk4, wk5, wk6, wk7};

    auto evaluate = [&](const Value& point) -> Value {
        if (evaluations >= maxEvaluations) {
            throw std::runtime_error(
                "GaussKronrod15: maximum number of function evaluations "
                "exceeded.");
        }

        ++evaluations;
        return evaluateAt(prepared, variable, point);
    };

    const Value fCenter = evaluate(center);

    Value kronrodSum = wk8 * fCenter;
    Value gaussSum = wg4 * fCenter;

    for (std::size_t i = 0; i < 7; ++i) {
        const Value offset = halfWidth * x[i];

        const Value fPlus = evaluate(center + offset);

        const Value fMinus = evaluate(center - offset);

        const Value pair = fPlus + fMinus;

        kronrodSum = kronrodSum + wk[i] * pair;

        // Gauss abscissae are x2, x4 and x6.
        if (i == 1) {
            gaussSum = gaussSum + wg1 * pair;
        } else if (i == 3) {
            gaussSum = gaussSum + wg2 * pair;
        } else if (i == 5) {
            gaussSum = gaussSum + wg3 * pair;
        }
    }

    const Value kronrod = halfWidth * kronrodSum;

    const Value gauss = halfWidth * gaussSum;

    const Storage error = std::abs((kronrod - gauss).real().value());

    return QuadratureResult{kronrod, gauss, error};
}

Value adaptiveIntegrate(const PreparedAst& prepared,
                        const std::string& variable, const Value& lower,
                        const Value& upper, const GaussKronrod15Config& config,
                        std::size_t& evaluations) {
    const QuadratureResult result = quadrature(
        prepared, variable, lower, upper, evaluations, config.maxEvaluations);

    const Storage magnitude = std::abs(result.kronrod.real().value());

    const Storage absoluteTolerance = config.absoluteTolerance.real().value();

    const Storage relativeTolerance = config.relativeTolerance.real().value();

    const Storage allowed = absoluteTolerance + relativeTolerance * magnitude;

    if (result.error <= allowed) {
        return result.kronrod;
    }

    const Value middle = (lower + upper) / Value(2.0);

    return adaptiveIntegrate(prepared, variable, lower, middle, config,
                             evaluations) +
           adaptiveIntegrate(prepared, variable, middle, upper, config,
                             evaluations);
}

}  // namespace

Value GaussKronrod15::integrate(const PreparedAst& prepared,
                                const std::string& variable, const Value& lower,
                                const Value& upper,
                                const GaussKronrod15Config& config) {
    const Storage absoluteTolerance = config.absoluteTolerance.real().value();

    const Storage relativeTolerance = config.relativeTolerance.real().value();

    if (absoluteTolerance <= 0.0) {
        throw std::invalid_argument(
            "GaussKronrod15: absoluteTolerance must be positive.");
    }

    if (relativeTolerance <= 0.0) {
        throw std::invalid_argument(
            "GaussKronrod15: relativeTolerance must be positive.");
    }

    if (config.maxEvaluations == 0) {
        throw std::invalid_argument(
            "GaussKronrod15: maxEvaluations must be greater than zero.");
    }

    if (lower == upper) {
        return Value{};
    }

    if (lower.real().value() > upper.real().value()) {
        return -integrate(prepared, variable, upper, lower, config);
    }

    std::size_t evaluations = 0;

    return adaptiveIntegrate(prepared, variable, lower, upper, config,
                             evaluations);
}

}  // namespace numathap::backend::integrate

