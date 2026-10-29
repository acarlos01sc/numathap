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

//[[nodiscard]]
//Storage absolute(Storage value) noexcept {
//    return value < Storage{0} ? -value : value;
//}

//[[nodiscard]]
//bool approximatelyEqual(Storage lhs, Storage rhs) noexcept {
//    constexpr Storage toleranceFactor = Storage{100};
//
//    const Storage epsilon = numeric::Real::epsilon();
//    const Storage absoluteTolerance = toleranceFactor * epsilon;
//    const Storage relativeTolerance = toleranceFactor * epsilon;
//
//    const Storage difference = absolute(lhs - rhs);
//
//    const Storage lhsMagnitude = absolute(lhs);
//    const Storage rhsMagnitude = absolute(rhs);
//    const Storage scale =
//        lhsMagnitude > rhsMagnitude ? lhsMagnitude : rhsMagnitude;
//
//    const Storage tolerance = absoluteTolerance > relativeTolerance * scale
//                                  ? absoluteTolerance
//                                  : relativeTolerance * scale;
//
//    return difference <= tolerance;
//}

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

        // Inserir o ponto no contexto recebido.
        context.setValue(variable, pointValue.str());

        // Avaliar a expressão normalizada.
        const core::Value value = backend::evaluate(normalized, context);

        if (value.isReal() && !value.real().isFinite()) {
            return LimitResult{};
        }

        return LimitResult{value, LimitStatus::Finite};

    } catch (const std::exception&) {
        return LimitResult{};
    }
}

}  // namespace numathap::backend::limit
