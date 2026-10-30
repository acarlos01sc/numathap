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

// Interpretar o ponto e identificar a lateralidade.
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

// Primeira estratégia: substituição direta.
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

LimitResult limit(const math::PreparedAst& expression,
                  const std::string& variable, const std::string& point) {
    core::Context context;
    return limit(expression, variable, point, context);
}

LimitResult limit(const math::PreparedAst& expression,
                  const std::string& variable, const std::string& point,
                  core::Context& context) {
    try {
        // Interpretar o ponto e identificar a lateralidade.
        ParsedPoint parsedPoint;

        if (!parsePoint(point, parsedPoint)) {
            return LimitResult{};
        }

        // Avaliar o ponto de aproximação.
        const core::Value pointValue = BackendSupport::evaluateConstant(
            parsedPoint.value, context, expression.environment());

        if (!pointValue.isReal() || !pointValue.real().isFinite()) {
            return LimitResult{};
        }

        // Criar a árvore de trabalho.
        math::MathAst workingTree(
            expression.expression(),
            BackendSupport::cloneNode(*expression.root()));

        // Normalizar a árvore de trabalho.
        const symbolic::ConstantFolder constantFolder;
        workingTree = constantFolder.fold(workingTree);

        const symbolic::Simplifier simplifier;
        workingTree = simplifier.simplify(workingTree);

        const symbolic::UltraSimplifier ultraSimplifier;
        workingTree = ultraSimplifier.simplify(workingTree);

        if (workingTree.empty()) {
            return LimitResult{};
        }

        // Preparar a árvore normalizada para avaliação.
        math::PreparedAst normalized(
            workingTree.expression(),
            BackendSupport::cloneNode(*workingTree.root()),
            expression.environment());

        // Executar as estratégias de resolução em sequência.

        // 1. Substituição direta.
        LimitResult result = calculateDirectSubstitution(
            normalized, variable, pointValue, parsedPoint.approach, context);

        if (result.status != LimitStatus::Failed) {
            return result;
        }

        // 2. Resolução algébrica de singularidades.
        result = internal::calculateAlgebraicSingularity(normalized, variable, pointValue,
                                               parsedPoint.approach, context);

        if (result.status != LimitStatus::Failed) {
            return result;
        }

        // 3. Futuras estratégias de resolução numérica.
        return LimitResult{};

    } catch (const std::exception&) {
        return LimitResult{};
    }
}

}  // namespace numathap::backend::limit