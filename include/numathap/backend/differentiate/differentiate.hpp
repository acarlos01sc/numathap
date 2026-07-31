#pragma once

#include <string>

#include "numathap/math/PreparedAst.hpp"

namespace numathap::backend::differentiate {

/**
 * @brief Computes the symbolic derivative of a prepared mathematical
 * expression.
 *
 * @param expression Prepared mathematical expression.
 * @param variable Name of the differentiation variable.
 * @return A prepared AST representing the simplified derivative.
 */
[[nodiscard]]
math::PreparedAst differentiate(const math::PreparedAst& expression,
                                const std::string& variable);

}  // namespace numathap::backend::differentiate