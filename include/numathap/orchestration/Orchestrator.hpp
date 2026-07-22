/**
 * @file Orchestrator.hpp
 * @brief Defines the Orchestrator class responsible for transforming Math-ASTs.
 *
 * This file contains the declaration of the Orchestrator, which acts as a
 * bridge between raw mathematical expressions and prepared structures optimized
 * for evaluation.
 */
#pragma once

#include "numathap/config/MathEnvironment.hpp"
#include "numathap/math/MathAst.hpp"

namespace numathap::orchestration {

/**
 * @brief Orchestrates the transformation of raw Math-ASTs into prepared ASTs.
 *
 * The Orchestrator acts as a processing engine that consumes a raw Math-AST and
 * generates an optimized or prepared tree suitable for subsequent evaluation.
 * The transformation process is strictly governed by the provided
 * MathEnvironment.
 *
 * @note This class ensures the input MathAst remains strictly immutable.
 */
class Orchestrator {
   public:
    Orchestrator() = default;
    ~Orchestrator() = default;

    /**
     * @brief Transforms a raw Math-AST into a prepared AST.
     *
     * Traverses the source AST and applies transformations based on the
     * enabled capabilities in the environment (e.g., constant folding,
     * symbolic simplification).
     *
     * @param mathAst The root of the source Math-AST.
     * @param environment Configuration parameters for the transformation
     * process.
     *
     * @return A pointer to the root node of the resulting prepared AST.
     */
    [[nodiscard]]
    math::MathNodePtr build(const math::MathAst& mathAst,
                            const config::MathEnvironment& environment) const;

   private:
    /**
     * @brief Applies the capabilities enabled in the environment.
     *
     * A working copy of the original Math-AST is progressively transformed by
     * the enabled capabilities. If no capabilities are enabled, the returned
     * tree is simply a structural copy of the original.
     *
     * @param mathAst The original immutable Math-AST.
     * @param environment Active processing environment.
     *
     * @return The transformed Math-AST.
     */
    [[nodiscard]]
    math::MathAst applyCapabilities(
        const math::MathAst& mathAst,
        const config::MathEnvironment& environment) const;

    /**
     * @brief Internal dispatcher that delegates node creation based on the node
     * type.
     * @internal This is part of the internal visitor/dispatch mechanism.
     */
    [[nodiscard]]
    math::MathNodePtr buildNode(
        const math::MathNode& node,
        const config::MathEnvironment& environment) const;

    /**
     * @brief Handles the construction of Number Nodes.
     * @internal Node-specific transformation logic.
     */
    [[nodiscard]]
    math::MathNodePtr buildNumber(
        const math::NumberNode& node,
        const config::MathEnvironment& environment) const;

    /**
     * @brief Processes and prepares a SymbolNode.
     * @internal Node-specific transformation logic.
     */
    [[nodiscard]]
    math::MathNodePtr buildSymbol(
        const math::SymbolNode& node,
        const config::MathEnvironment& environment) const;

    /**
     * @brief Processes and prepares a UnaryNode, recursively building its
     * operand.
     * @internal Node-specific transformation logic.
     */
    [[nodiscard]]
    math::MathNodePtr buildUnary(
        const math::UnaryNode& node,
        const config::MathEnvironment& environment) const;

    /**
     * @brief Processes and prepares a UnaryNode, recursively building its
     * operand.
     * @internal Node-specific transformation logic.
     */
    [[nodiscard]]
    math::MathNodePtr buildBinary(
        const math::BinaryNode& node,
        const config::MathEnvironment& environment) const;

    /**
     * @brief Processes and prepares a FunctionNode, recursively building its
     * arguments.
     * @internal Node-specific transformation logic.
     */
    [[nodiscard]]
    math::MathNodePtr buildFunction(
        const math::FunctionNode& node,
        const config::MathEnvironment& environment) const;
};

}  // namespace numathap::orchestration