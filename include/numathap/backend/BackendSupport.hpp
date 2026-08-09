/**
 * @file BackendSupport.hpp
 * @brief Shared utility services for backend implementations.
 *
 * This class provides common helper services used by backend algorithms,
 * avoiding code duplication between evaluators, integrators, differentiators,
 * series generators and future symbolic or numerical backends.
 */

#pragma once

#include <string>

#include "numathap/config/MathEnvironment.hpp"
#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"
#include "numathap/math/MathNode.hpp"

namespace numathap::backend {

/**
 * @brief Common infrastructure services shared by backend implementations.
 *
 * This class centralizes operations that are useful across multiple backends,
 * such as evaluating constant expressions, parsing numerical values and other
 * backend-independent support routines.
 *
 * All functions are stateless and thread-safe.
 */
class BackendSupport {
   public:
    BackendSupport() = delete;

    /**
     * @brief Evaluates a constant expression.
     *
     * The expression may be:
     * - a numeric literal;
     * - a mathematical constant (for example @c pi or @c e);
     * - a constant mathematical expression (for example @c pi/2 or
     *   @c sqrt(2));
     * - a symbol defined in the supplied context.
     *
     * @param expression Constant expression to evaluate.
     * @param context Context used to resolve symbols.
     * @param environment Mathematical environment used during evaluation.
     *
     * @return Evaluated numerical value.
     *
     * @throw std::invalid_argument If the expression cannot be evaluated.
     */
    [[nodiscard]]
    static core::Value evaluateConstant(
        const std::string& expression, const core::Context& context,
        const config::MathEnvironment& environment);

    /**
     * @brief Creates a deep copy of a MathNode tree.
     *
     * Recursively clones every supported node type (NumberNode, SymbolNode,
     * UnaryNode, BinaryNode, FunctionNode) via numathap::dispatch::Dispatcher,
     * producing a tree that is structurally identical to and fully
     * independent from @p node.
     *
     * @param node Root of the tree to clone.
     *
     * @return A new MathNode tree, structurally identical to @p node.
     */
    [[nodiscard]]
    static math::MathNodePtr cloneNode(const math::MathNode& node);
};

}  // namespace numathap::backend