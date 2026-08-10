#include "numathap/orchestration/Orchestrator.hpp"

#include <stdexcept>

#include "numathap/config/Capability.hpp"
#include "numathap/symbolic/ConstantFolder.hpp"
#include "numathap/symbolic/Simplifier.hpp"
#include "numathap/symbolic/UltraSimplifier.hpp"

namespace numathap::orchestration {

using namespace numathap::math;

MathNodePtr Orchestrator::build(
    const MathAst& mathAst, const config::MathEnvironment& environment) const {
    if (mathAst.root() == nullptr) {
        return nullptr;
    }

    MathAst processedAst = applyCapabilities(mathAst, environment);

    if (processedAst.root() == nullptr) {
        return nullptr;
    }

    return buildNode(*processedAst.root(), environment);
}

MathAst Orchestrator::applyCapabilities(
    const MathAst& mathAst, const config::MathEnvironment& environment) const {
    MathAst currentAst(mathAst.expression(),
                       buildNode(*mathAst.root(), environment));

    for (const auto capability : environment.capabilities()) {
        switch (capability) {
            case config::Capability::ConstantFolder: {
                symbolic::ConstantFolder folder;
                currentAst = folder.fold(currentAst);
                break;
            }

            case config::Capability::Simplify: {
                symbolic::Simplifier simplifier;
                currentAst = simplifier.simplify(currentAst);
                break;
            }

            case config::Capability::UltraSimplifier: {
                symbolic::UltraSimplifier simplifier;
                currentAst = simplifier.simplify(currentAst);
                break;
            }
        }
    }

    return currentAst;
}

MathNodePtr Orchestrator::buildNode(
    const MathNode& node, const config::MathEnvironment& environment) const {
    if (auto p = dynamic_cast<const NumberNode*>(&node)) {
        return buildNumber(*p, environment);
    }

    if (auto p = dynamic_cast<const SymbolNode*>(&node)) {
        return buildSymbol(*p, environment);
    }

    if (auto p = dynamic_cast<const UnaryNode*>(&node)) {
        return buildUnary(*p, environment);
    }

    if (auto p = dynamic_cast<const BinaryNode*>(&node)) {
        return buildBinary(*p, environment);
    }

    if (auto p = dynamic_cast<const FunctionNode*>(&node)) {
        return buildFunction(*p, environment);
    }

    throw std::runtime_error("Unknown MathNode type.");
}

MathNodePtr Orchestrator::buildNumber(const NumberNode& node,
                                      const config::MathEnvironment&) const {
    //
    // Future capabilities may transform this node.
    //

    return std::make_unique<NumberNode>(node.value);
}

MathNodePtr Orchestrator::buildSymbol(const SymbolNode& node,
                                      const config::MathEnvironment&) const {
    //
    // Future capabilities may transform this node.
    //

    return std::make_unique<SymbolNode>(node.name);
}

MathNodePtr Orchestrator::buildUnary(
    const UnaryNode& node, const config::MathEnvironment& environment) const {
    auto operand = buildNode(*node.operand, environment);

    //
    // Future capabilities may rewrite this subtree.
    //

    return std::make_unique<UnaryNode>(node.op, std::move(operand));
}

MathNodePtr Orchestrator::buildBinary(
    const BinaryNode& node, const config::MathEnvironment& environment) const {
    auto left = buildNode(*node.left, environment);

    auto right = buildNode(*node.right, environment);

    //
    // Future capabilities may rewrite this subtree.
    //

    return std::make_unique<BinaryNode>(node.op, std::move(left),
                                        std::move(right));
}

MathNodePtr Orchestrator::buildFunction(
    const FunctionNode& node,
    const config::MathEnvironment& environment) const {
    std::vector<MathNodePtr> arguments;

    arguments.reserve(node.arguments.size());

    for (const auto& argument : node.arguments) {
        arguments.push_back(buildNode(*argument, environment));
    }

    //
    // Future capabilities may rewrite this function.
    //

    return std::make_unique<FunctionNode>(node.name, std::move(arguments));
}

}  // namespace numathap::orchestration