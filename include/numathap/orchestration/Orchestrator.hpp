#pragma once

#include "numathap/config/MathEnvironment.hpp"
#include "numathap/math/MathAst.hpp"

namespace numathap::orchestration {

/**
 * @brief Builds a Prepared-AST from a Math-AST.
 *
 * The Orchestrator traverses the Math-AST and constructs a new tree.
 * During this process it may apply the capabilities enabled in the
 * MathEnvironment.
 *
 * The source Math-AST is never modified.
 */
class Orchestrator {
public:

    Orchestrator() = default;
    ~Orchestrator() = default;

    /**
     * @brief Builds a Prepared-AST tree.
     *
     * @param mathAst Source Math-AST.
     * @param environment Preparation environment.
     *
     * @return Root node of the Prepared-AST.
     */
    [[nodiscard]]
    math::MathNodePtr build(
        const math::MathAst& mathAst,
        const config::MathEnvironment& environment) const;

private:

    [[nodiscard]]
    math::MathNodePtr buildNode(
        const math::MathNode& node,
        const config::MathEnvironment& environment) const;

    [[nodiscard]]
    math::MathNodePtr buildNumber(
        const math::NumberNode& node,
        const config::MathEnvironment& environment) const;

    [[nodiscard]]
    math::MathNodePtr buildSymbol(
        const math::SymbolNode& node,
        const config::MathEnvironment& environment) const;

    [[nodiscard]]
    math::MathNodePtr buildUnary(
        const math::UnaryNode& node,
        const config::MathEnvironment& environment) const;

    [[nodiscard]]
    math::MathNodePtr buildBinary(
        const math::BinaryNode& node,
        const config::MathEnvironment& environment) const;

    [[nodiscard]]
    math::MathNodePtr buildFunction(
        const math::FunctionNode& node,
        const config::MathEnvironment& environment) const;

};

} // namespace numathap::orchestration