#include "numathap/backend/limit/limit.hpp"

#include <exception>
#include <string>

#include "limit_internal.hpp"
#include "numathap/backend/BackendSupport.hpp"
#include "numathap/backend/evaluate.hpp"
#include "numathap/core/Context.hpp"
#include "numathap/symbolic/ConstantFolder.hpp"
#include "numathap/symbolic/Simplifier.hpp"
#include "numathap/symbolic/UltraSimplifier.hpp"

namespace numathap::backend::limit {

namespace {

struct ParsedPoint {
    std::string value;
    internal::Approach approach{internal::Approach::Bilateral};
};

// Parse the approach point and determine the requested direction.
bool parsePoint(const std::string& point, ParsedPoint& parsed) {
    if (point.empty() || point == "+inf" || point == "-inf") {
        return false;
    }

    parsed.value = point;
    parsed.approach = internal::Approach::Bilateral;

    if (parsed.value.back() == '+') {
        parsed.value.pop_back();
        parsed.approach = internal::Approach::FromRight;
    } else if (parsed.value.back() == '-') {
        parsed.value.pop_back();
        parsed.approach = internal::Approach::FromLeft;
    }

    return !parsed.value.empty();
}

/**
 * @brief Evaluate the expression by direct substitution.
 *
 * @param normalized Normalized expression to evaluate.
 * @param variable Name of the variable approaching the point.
 * @param pointValue Finite real value of the approach point.
 * @param[in] approach Requested direction of approach (unused by this
 * strategy).
 * @param context Evaluation context.
 * @return A finite result when direct substitution succeeds; otherwise, a
 *         failed result so that the next strategy can be attempted.
 */
LimitResult calculateDirectSubstitution(const math::PreparedAst& normalized,
                                        const std::string& variable,
                                        const core::Value& pointValue,
                                        internal::Approach /*approach*/,
                                        core::Context& context) {
    try {
        context.setValue(variable, pointValue.str());

        const core::Value value = backend::evaluate(normalized, context);

        if (value.isReal() && !value.real().isFinite()) {
            return LimitResult{};
        }

        return LimitResult{value, LimitStatus::Finite};

    } catch (const std::exception&) {
        return LimitResult{};
    }
}

}  // namespace

/**
 * @brief Compute the limit of an expression using a local evaluation context.
 *
 * @param expression Prepared expression whose limit is requested.
 * @param variable Name of the variable approaching the point.
 * @param point Approach point. Append `+` for a right-hand limit or `-` for a
 *        left-hand limit; omit the suffix for a two-sided limit.
 * @return The computed limit result, or a failed result if no available
 *         strategy can determine the limit.
 *
 * @details This overload creates a context and delegates the calculation to
 * the overload that accepts an explicit context.
 */
LimitResult limit(const math::PreparedAst& expression,
                  const std::string& variable, const std::string& point) {
    core::Context context;
    return limit(expression, variable, point, context);
}

/**
 * @brief Compute the limit of an expression using the supplied context.
 *
 * @param expression Prepared expression whose limit is requested.
 * @param variable Name of the variable approaching the point.
 * @param point Approach point. Append `+` for a right-hand limit or `-` for a
 *        left-hand limit; omit the suffix for a two-sided limit.
 * @param context Context used to evaluate the approach point and expression.
 * @return A `LimitResult` describing the finite value, divergence, nonexistence,
 *         or failure to determine the limit.
 *
 * @details The expression is cloned and normalized before the available
 * strategies are attempted. Strategies are executed in order: direct
 * substitution, followed by algebraic singularity analysis. A failed strategy
 * allows the next strategy to run.
 *
 * @note The current implementation accepts finite real approach points.
 * Infinite approach points are not handled by this backend.
 */
LimitResult limit(const math::PreparedAst& expression,
                  const std::string& variable, const std::string& point,
                  core::Context& context) {
    try {
        // Parse the approach point and determine the requested direction.
        ParsedPoint parsedPoint;

        if (!parsePoint(point, parsedPoint)) {
            return LimitResult{};
        }

        // Evaluate the approach point.
        const core::Value pointValue = BackendSupport::evaluateConstant(
            parsedPoint.value, context, expression.environment());

        if (!pointValue.isReal() || !pointValue.real().isFinite()) {
            return LimitResult{};
        }

        // Create a working copy of the expression tree.
        math::MathAst workingTree(
            expression.expression(),
            BackendSupport::cloneNode(*expression.root()));

        // Normalize the working expression tree.
        const symbolic::ConstantFolder constantFolder;
        workingTree = constantFolder.fold(workingTree);

        const symbolic::Simplifier simplifier;
        workingTree = simplifier.simplify(workingTree);

        const symbolic::UltraSimplifier ultraSimplifier;
        workingTree = ultraSimplifier.simplify(workingTree);

        if (workingTree.empty()) {
            return LimitResult{};
        }

        // Prepare the normalized tree for evaluation.
        math::PreparedAst normalized(
            workingTree.expression(),
            BackendSupport::cloneNode(*workingTree.root()),
            expression.environment());

        // Run the available limit-evaluation strategies in sequence.

        // 1. Direct substitution.
        LimitResult result = calculateDirectSubstitution(
            normalized, variable, pointValue, parsedPoint.approach, context);

        if (result.status != LimitStatus::Failed) {
            return result;
        }

        // 2. Algebraic singularity analysis.
        result = internal::calculateAlgebraicSingularity(
            normalized, variable, pointValue, parsedPoint.approach, context);

        if (result.status != LimitStatus::Failed) {
            return result;
        }

        // 3. Future numerical strategies may be added here.
        return LimitResult{};

    } catch (const std::exception&) {
        return LimitResult{};
    }
}

}  // namespace numathap::backend::limit