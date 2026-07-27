/**
 * @file ImproperIntegralTransformer.cpp
 * @brief Implements the tangent substitution for improper integrals.
 */

#include "numathap/backend/integrate/ImproperIntegralTransformer.hpp"

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "numathap/math/MathAst.hpp"
#include "numathap/math/MathNode.hpp"

namespace numathap::backend::integrate {

using config::MathEnvironment;
using math::BinaryNode;
using math::BinaryOp;
using math::FunctionNode;
using math::MathAst;
using math::MathNode;
using math::MathNodePtr;
using math::NumberNode;
using math::PreparedAst;
using math::SymbolNode;
using math::UnaryNode;

namespace {

constexpr std::string_view InternalVariablePrefix = "__numathap_u";

}  // namespace

TransformedIntegral ImproperIntegralTransformer::transform(
    const PreparedAst& prepared, const std::string& variable,
    const MathEnvironment& environment) const {
    if (prepared.empty()) {
        throw std::invalid_argument("Cannot transform an empty PreparedAst.");
    }

    if (variable.empty()) {
        throw std::invalid_argument("Integration variable cannot be empty.");
    }

    const std::string internalVariable = generateInternalVariable(prepared);

    MathNodePtr transformedRoot =
        transformNode(*prepared.root(), variable, internalVariable);

    /*
     * The tangent substitution changes:
     *
     *     f(x)
     *
     * into:
     *
     *     f(tan(u)) / cos(u)^2
     *
     * The Jacobian is applied only to the root expression.
     */
    transformedRoot =
        applyJacobian(std::move(transformedRoot), internalVariable);

    MathAst transformedAst(prepared.expression(), std::move(transformedRoot));

    PreparedAst transformedPrepared(transformedAst, environment);

    return TransformedIntegral{std::move(transformedPrepared),
                               internalVariable};
}

std::string ImproperIntegralTransformer::generateInternalVariable(
    const PreparedAst& prepared) const {
    std::string candidate{InternalVariablePrefix};

    std::size_t index = 0;

    while (containsSymbol(*prepared.root(), candidate)) {
        ++index;
        candidate =
            std::string(InternalVariablePrefix) + "_" + std::to_string(index);
    }

    return candidate;
}

bool ImproperIntegralTransformer::containsSymbol(
    const MathNode& node, const std::string& symbol) const {
    if (const auto* n = dynamic_cast<const SymbolNode*>(&node)) {
        return n->name == symbol;
    }

    if (const auto* n = dynamic_cast<const UnaryNode*>(&node)) {
        return containsSymbol(*n->operand, symbol);
    }

    if (const auto* n = dynamic_cast<const BinaryNode*>(&node)) {
        return containsSymbol(*n->left, symbol) ||
               containsSymbol(*n->right, symbol);
    }

    if (const auto* n = dynamic_cast<const FunctionNode*>(&node)) {
        for (const auto& argument : n->arguments) {
            if (containsSymbol(*argument, symbol)) {
                return true;
            }
        }

        return false;
    }

    if (dynamic_cast<const NumberNode*>(&node)) {
        return false;
    }

    throw std::logic_error(
        "Unsupported MathNode type while searching for symbol.");
}

MathNodePtr ImproperIntegralTransformer::transformNode(
    const MathNode& node, const std::string& integrationVariable,
    const std::string& internalVariable) const {
    //----------------------------------------------------------
    // Number
    //----------------------------------------------------------

    if (const auto* n = dynamic_cast<const NumberNode*>(&node)) {
        return std::make_unique<NumberNode>(n->value);
    }

    //----------------------------------------------------------
    // Symbol
    //----------------------------------------------------------

    if (const auto* n = dynamic_cast<const SymbolNode*>(&node)) {
        if (n->name == integrationVariable) {
            return makeTangentNode(internalVariable);
        }

        return std::make_unique<SymbolNode>(n->name);
    }

    //----------------------------------------------------------
    // Unary
    //----------------------------------------------------------

    if (const auto* n = dynamic_cast<const UnaryNode*>(&node)) {
        return std::make_unique<UnaryNode>(
            n->op,
            transformNode(*n->operand, integrationVariable, internalVariable));
    }

    //----------------------------------------------------------
    // Binary
    //----------------------------------------------------------

    if (const auto* n = dynamic_cast<const BinaryNode*>(&node)) {
        return std::make_unique<BinaryNode>(
            n->op,
            transformNode(*n->left, integrationVariable, internalVariable),
            transformNode(*n->right, integrationVariable, internalVariable));
    }

    //----------------------------------------------------------
    // Function
    //----------------------------------------------------------

    if (const auto* n = dynamic_cast<const FunctionNode*>(&node)) {
        std::vector<MathNodePtr> arguments;
        arguments.reserve(n->arguments.size());

        for (const auto& argument : n->arguments) {
            arguments.push_back(transformNode(*argument, integrationVariable,
                                              internalVariable));
        }

        return std::make_unique<FunctionNode>(n->name, std::move(arguments));
    }

    throw std::logic_error(
        "Unsupported MathNode type during improper integral "
        "transformation.");
}

MathNodePtr ImproperIntegralTransformer::makeTangentNode(
    const std::string& internalVariable) const {
    std::vector<MathNodePtr> arguments;

    arguments.push_back(std::make_unique<SymbolNode>(internalVariable));

    return std::make_unique<FunctionNode>("tan", std::move(arguments));
}

MathNodePtr ImproperIntegralTransformer::makeJacobianNode(
    const std::string& internalVariable) const {
    /*
     * Build:
     *
     *     1 / cos(u)^2
     */

    std::vector<MathNodePtr> cosArguments;

    cosArguments.push_back(std::make_unique<SymbolNode>(internalVariable));

    auto cosine =
        std::make_unique<FunctionNode>("cos", std::move(cosArguments));

    auto exponent = std::make_unique<NumberNode>("2");

    auto cosineSquared = std::make_unique<BinaryNode>(
        BinaryOp::Power, std::move(cosine), std::move(exponent));

    auto numerator = std::make_unique<NumberNode>("1");

    return std::make_unique<BinaryNode>(BinaryOp::Divide, std::move(numerator),
                                        std::move(cosineSquared));
}

MathNodePtr ImproperIntegralTransformer::applyJacobian(
    MathNodePtr transformedExpression,
    const std::string& internalVariable) const {
    if (!transformedExpression) {
        throw std::invalid_argument(
            "Cannot apply Jacobian to an empty expression.");
    }

    return std::make_unique<BinaryNode>(BinaryOp::Multiply,
                                        std::move(transformedExpression),
                                        makeJacobianNode(internalVariable));
}

}  // namespace numathap::backend::integrate