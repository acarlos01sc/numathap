#pragma once

#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"
#include "numathap/math/MathNode.hpp"
#include "numathap/math/PreparedAst.hpp"

namespace numathap::backend {

/**
 * @brief Stateless numerical evaluator.
 *
 * Evaluates a PreparedAst using the MathAdapter associated with the
 * PreparedAst itself.
 */
class Evaluator {
public:
    /**
     * @brief Evaluates a prepared mathematical expression.
     *
     * @param prepared Prepared expression.
     * @param context Evaluation context.
     *
     * @return Result of the evaluation.
     */
    [[nodiscard]]
    static core::Value evaluate(
        const math::PreparedAst& prepared,
        const core::Context& context);

    // Dispatcher visitors

    [[nodiscard]]
    core::Value operator()(const math::NumberNode& node) const;

    [[nodiscard]]
    core::Value operator()(const math::SymbolNode& node) const;

    [[nodiscard]]
    core::Value operator()(const math::UnaryNode& node) const;

    [[nodiscard]]
    core::Value operator()(const math::BinaryNode& node) const;

    [[nodiscard]]
    core::Value operator()(const math::FunctionNode& node) const;

private:
    Evaluator(const math::PreparedAst& prepared,
              const core::Context& context);

    [[nodiscard]]
    core::Value dispatch(const math::MathNode& node) const;

    [[nodiscard]]
    core::Value resolveSymbol(const std::string& symbol) const;

    [[nodiscard]]
    core::Value parseValue(const std::string& text) const;

private:
    const math::PreparedAst& prepared_;
    const core::Context& context_;
};

} // namespace numathap::backend