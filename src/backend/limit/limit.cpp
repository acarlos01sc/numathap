#include "numathap/backend/limit/limit.hpp"

#include <exception>

#include "numathap/backend/BackendSupport.hpp"
#include "numathap/backend/evaluate.hpp"
#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"
#include "numathap/symbolic/ConstantFolder.hpp"
#include "numathap/symbolic/Simplifier.hpp"
#include "numathap/symbolic/UltraSimplifier.hpp"

namespace numathap::backend::limit {

namespace {

using Storage = numeric::Real::Storage;

[[nodiscard]]
LimitResult calculateDirectSubstitution(const math::PreparedAst& expression,
                                        const std::string& variable,
                                        const core::Value& point,
                                        core::Context& context) {
    context.setValue(variable, point.str());

    const core::Value value = backend::evaluate(expression, context);

    if (value.isReal() && !value.real().isFinite()) {
        return LimitResult{};
    }

    return LimitResult{value, LimitStatus::Finite};
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
        // Rejeitar pontos ainda não suportados.
        if (point.empty() || point == "+inf" || point == "-inf" ||
            point.back() == '+' || point.back() == '-') {
            return LimitResult{};
        }

        // Validar o ponto de aproximação.
        const core::Value pointValue = BackendSupport::evaluateConstant(
            point, context, expression.environment());

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

        return calculateDirectSubstitution(normalized, variable, pointValue,
                                           context);

    } catch (const std::exception&) {
        return LimitResult{};
    }
}

}  // namespace numathap::backend::limit
