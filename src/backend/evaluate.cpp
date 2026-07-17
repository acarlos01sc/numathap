#include "numathap/backend/evaluate.hpp"

#include "numathap/backend/Evaluator.hpp"

namespace numathap::backend {

core::Value evaluate(
    const math::PreparedAst& expression,
    const core::Context& context)
{
    return backend::Evaluator::evaluate(expression, context);
}

} // namespace numathap::backend