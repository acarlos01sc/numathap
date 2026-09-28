#include "numathap/backend/evaluate_cx.hpp"

#include "numathap/backend/Evaluator_cx.hpp"

namespace numathap::backend {

core::Value evaluate_cx(
    const math::PreparedAst& expression,
    const core::Context& context) {

  return Evaluator_cx::evaluate(expression, context);
}

} // namespace numathap::backend