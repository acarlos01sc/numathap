#include "numathap/backend/limit/limit.hpp"

#include "numathap/backend/BackendSupport.hpp"
#include "numathap/backend/evaluate.hpp"
#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"
#include "numathap/symbolic/ConstantFolder.hpp"
#include "numathap/symbolic/Simplifier.hpp"
#include "numathap/symbolic/UltraSimplifier.hpp"

#include <exception>

namespace numathap::backend::limit {

namespace {

[[nodiscard]]
math::MathAst normalize(const math::PreparedAst& expression) {
    math::MathAst workingAst(
        expression.expression(),
        BackendSupport::cloneNode(*expression.root())
    );

    const symbolic::ConstantFolder constantFolder;
    workingAst = constantFolder.fold(workingAst);

    const symbolic::Simplifier simplifier;
    workingAst = simplifier.simplify(workingAst);

    const symbolic::UltraSimplifier ultraSimplifier;
    workingAst = ultraSimplifier.simplify(workingAst);

    return workingAst;
}

}  // namespace

LimitResult limit(
    const math::PreparedAst& expression,
    const std::string& variable,
    const std::string& point
) {
    const core::Context context;
    return limit(expression, variable, point, context);
}

LimitResult limit(
    const math::PreparedAst& expression,
    const std::string& variable,
    const std::string& point,
    const core::Context& context
) {
    try {
        // Esta etapa aceita somente pontos reais finitos.
        const core::Value pointValue = core::Value::parse(point);

        if (!pointValue.isReal() || !pointValue.real().isFinite()) {
            return LimitResult{};
        }

        if (expression.empty() || expression.root() == nullptr) {
            return LimitResult{};
        }

        // Normaliza uma cópia independente da AST original.
        auto normalizedAst = normalize(expression);

        if (normalizedAst.empty() || normalizedAst.root() == nullptr) {
            return LimitResult{};
        }

        // A árvore já está normalizada; não reaplicamos as capacidades
        // do ambiente original.
        math::PreparedAst normalizedExpression(
            normalizedAst.expression(),
            BackendSupport::cloneNode(*normalizedAst.root()),
            expression.environment()
        );

        // Preserva o contexto original, mas define a variável de limite
        // para o ponto em que estamos avaliando.
        core::Context evaluationContext = context;
        evaluationContext.setValue(variable, point);

        // Reutiliza o avaliador público da biblioteca.
        const core::Value value =
            numathap::backend::evaluate(
                normalizedExpression,
                evaluationContext
            );

        // Só classificamos como finito um resultado real finito.
        if (!value.isReal() || !value.real().isFinite()) {
            return LimitResult{};
        }

        return LimitResult{
            value,
            LimitStatus::Finite
        };
    } catch (const std::exception&) {
        // Uma falha de parsing ou avaliação não prova que o limite
        // não existe.
        return LimitResult{};
    }
}

}  // namespace numathap::backend::limit
