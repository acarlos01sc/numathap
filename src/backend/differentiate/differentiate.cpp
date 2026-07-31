#include "numathap/backend/differentiate/differentiate.hpp"

#include "numathap/backend/differentiate/Differentiator.hpp"

namespace numathap::backend::differentiate {

math::PreparedAst differentiate(
    const math::PreparedAst& expression,
    const std::string& variable) {
    return Differentiator{}.differentiate(expression, variable);
}

}  // namespace numathap::backend::differentiate