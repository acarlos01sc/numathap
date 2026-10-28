#include "numathap/backend/limit/limit.hpp"

#include "numathap/backend/BackendSupport.hpp"
#include "numathap/symbolic/ConstantFolder.hpp"
#include "numathap/symbolic/Simplifier.hpp"
#include "numathap/symbolic/UltraSimplifier.hpp"

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
    [[maybe_unused]] auto normalizedAst = normalize(expression);

    // O cálculo do limite será implementado incrementalmente com TDD.
    (void)variable;
    (void)point;

    return LimitResult{};
}

LimitResult limit(
    const math::PreparedAst& expression,
    const std::string& variable,
    const std::string& point,
    const core::Context& context
) {
    // O contexto será utilizado nas etapas de cálculo do limite.
    (void)context;

    return limit(expression, variable, point);
}

}  // namespace numathap::backend::limit