/**
 * @file UltraSimplifier.cpp
 * @brief Implements the UltraSimplifier.
 */

#include "numathap/symbolic/UltraSimplifier.hpp"

#include <type_traits>
#include <utility>
#include <vector>

#include "numathap/core/Value.hpp"
#include "numathap/dispatch/Dispatcher.hpp"

namespace numathap::symbolic {

using namespace numathap::math;

MathAst UltraSimplifier::simplify(const MathAst& mathAst) const {
    if (mathAst.root() == nullptr) {
        return MathAst(mathAst.expression(), nullptr);
    }

    return MathAst(mathAst.expression(), simplifyNode(*mathAst.root()));
}

MathNodePtr UltraSimplifier::simplifyNode(const MathNode& node) const {
    return dispatch::Dispatcher::dispatch(
        node, [this](const auto& concreteNode) -> MathNodePtr {
            using NodeType = std::decay_t<decltype(concreteNode)>;

            if constexpr (std::is_same_v<NodeType, NumberNode>) {
                return std::make_unique<NumberNode>(concreteNode.value);

            } else if constexpr (std::is_same_v<NodeType, SymbolNode>) {
                return std::make_unique<SymbolNode>(concreteNode.name);

            } else if constexpr (std::is_same_v<NodeType, UnaryNode>) {
                return simplifyUnary(concreteNode);

            } else if constexpr (std::is_same_v<NodeType, BinaryNode>) {
                return simplifyBinary(concreteNode);

            } else if constexpr (std::is_same_v<NodeType, FunctionNode>) {
                return simplifyFunction(concreteNode);

            } else {
                static_assert(std::is_same_v<NodeType, void>,
                              "UltraSimplifier: unsupported MathNode type.");
            }
        });
}

MathNodePtr UltraSimplifier::simplifyUnary(const UnaryNode& node) const {
    auto operand = simplifyNode(*node.operand);

    return std::make_unique<UnaryNode>(node.op, std::move(operand));
}

MathNodePtr UltraSimplifier::simplifyBinary(const BinaryNode& node) const {
    //
    // Simplify children first.
    //
    auto left = simplifyNode(*node.left);
    auto right = simplifyNode(*node.right);

    return simplifyBinaryNode(node.op, std::move(left), std::move(right));
}

MathNodePtr UltraSimplifier::simplifyFunction(const FunctionNode& node) const {
    std::vector<MathNodePtr> arguments;
    arguments.reserve(node.arguments.size());

    for (const auto& argument : node.arguments) {
        arguments.push_back(simplifyNode(*argument));
    }

    return std::make_unique<FunctionNode>(node.name, std::move(arguments));
}

MathNodePtr UltraSimplifier::simplifyBinaryNode(BinaryOp op, MathNodePtr left,
                                                MathNodePtr right) const {
    if (op == BinaryOp::Divide) {
        return cancelCommonFactors(std::move(left), std::move(right));
    }

    return std::make_unique<BinaryNode>(op, std::move(left), std::move(right));
}

MathNodePtr UltraSimplifier::cancelCommonFactors(
    MathNodePtr numerator, MathNodePtr denominator) const {
    //
    // Do not perform cancellation when either complete side is zero.
    //
    // In particular:
    //
    //     0 / 0
    //
    // must not become 1.
    //
    if (isZero(*numerator) || isZero(*denominator)) {
        return std::make_unique<BinaryNode>(
            BinaryOp::Divide, std::move(numerator), std::move(denominator));
    }

    std::vector<MathNodePtr> numeratorFactors;
    std::vector<MathNodePtr> denominatorFactors;

    collectFactors(*numerator, numeratorFactors);
    collectFactors(*denominator, denominatorFactors);

    //
    // Cancel one occurrence at a time.
    //
    // This correctly handles repeated factors:
    //
    //     (x * x * y) / (x * x)
    //
    // becomes:
    //
    //     y
    //
    for (auto denominatorIt = denominatorFactors.begin();
         denominatorIt != denominatorFactors.end();) {
        bool cancelled = false;

        for (auto numeratorIt = numeratorFactors.begin();
             numeratorIt != numeratorFactors.end(); ++numeratorIt) {
            if (!equivalent(**numeratorIt, **denominatorIt)) {
                continue;
            }

            //
            // A zero factor must never be cancelled.
            //
            if (isZero(**numeratorIt)) {
                continue;
            }

            numeratorIt = numeratorFactors.erase(numeratorIt);

            denominatorIt = denominatorFactors.erase(denominatorIt);

            cancelled = true;
            break;
        }

        if (!cancelled) {
            ++denominatorIt;
        }
    }

    //
    // Nothing was cancelled.
    //
    if (numeratorFactors.size() == 1 && denominatorFactors.size() == 1) {
        if (equivalent(*numeratorFactors.front(),
                       *denominatorFactors.front())) {
            //
            // This is mostly defensive because the generic
            // cancellation loop normally handles this case.
            //
            return std::make_unique<NumberNode>("1");
        }
    }

    auto newNumerator = buildProduct(std::move(numeratorFactors));

    auto newDenominator = buildProduct(std::move(denominatorFactors));

    //
    // denominator == 1
    //
    if (isOne(*newDenominator)) {
        return newNumerator;
    }

    //
    // numerator == 1
    //
    if (isOne(*newNumerator)) {
        return std::make_unique<BinaryNode>(BinaryOp::Divide,
                                            std::move(newNumerator),
                                            std::move(newDenominator));
    }

    return std::make_unique<BinaryNode>(
        BinaryOp::Divide, std::move(newNumerator), std::move(newDenominator));
}

void UltraSimplifier::collectFactors(const MathNode& node,
                                     std::vector<MathNodePtr>& factors) const {
    //
    // Flatten multiplication:
    //
    //     A * (B * C)
    //
    // becomes:
    //
    //     A, B, C
    //
    dispatch::Dispatcher::dispatch(
        node, [this, &factors](const auto& concreteNode) {
            using NodeType = std::decay_t<decltype(concreteNode)>;

            if constexpr (std::is_same_v<NodeType, BinaryNode>) {
                if (concreteNode.op == BinaryOp::Multiply) {
                    collectFactors(*concreteNode.left, factors);

                    collectFactors(*concreteNode.right, factors);

                    return;
                }

                factors.push_back(simplifyNode(concreteNode));

            } else {
                factors.push_back(simplifyNode(concreteNode));
            }
        });
}

MathNodePtr UltraSimplifier::buildProduct(
    std::vector<MathNodePtr> factors) const {
    if (factors.empty()) {
        return std::make_unique<NumberNode>("1");
    }

    if (factors.size() == 1) {
        return std::move(factors.front());
    }

    MathNodePtr result = std::move(factors.front());

    for (std::size_t i = 1; i < factors.size(); ++i) {
        result = std::make_unique<BinaryNode>(
            BinaryOp::Multiply, std::move(result), std::move(factors[i]));
    }

    return result;
}

bool UltraSimplifier::equivalent(const MathNode& left,
                                 const MathNode& right) const {
    struct EquivalentVisitor {
        const UltraSimplifier* self;
        const MathNode& right;

        bool operator()(const NumberNode& leftNode) const {
            const auto* rightNode = dynamic_cast<const NumberNode*>(&right);

            return rightNode != nullptr && leftNode.value == rightNode->value;
        }

        bool operator()(const SymbolNode& leftNode) const {
            const auto* rightNode = dynamic_cast<const SymbolNode*>(&right);

            return rightNode != nullptr && leftNode.name == rightNode->name;
        }

        bool operator()(const UnaryNode& leftNode) const {
            const auto* rightNode = dynamic_cast<const UnaryNode*>(&right);

            return rightNode != nullptr && leftNode.op == rightNode->op &&
                   self->equivalent(*leftNode.operand, *rightNode->operand);
        }

        bool operator()(const BinaryNode& leftNode) const {
            const auto* rightNode = dynamic_cast<const BinaryNode*>(&right);

            return rightNode != nullptr && leftNode.op == rightNode->op &&
                   self->equivalent(*leftNode.left, *rightNode->left) &&
                   self->equivalent(*leftNode.right, *rightNode->right);
        }

        bool operator()(const FunctionNode& leftNode) const {
            const auto* rightNode = dynamic_cast<const FunctionNode*>(&right);

            if (rightNode == nullptr || leftNode.name != rightNode->name ||
                leftNode.arguments.size() != rightNode->arguments.size()) {
                return false;
            }

            for (std::size_t i = 0; i < leftNode.arguments.size(); ++i) {
                if (!self->equivalent(*leftNode.arguments[i],
                                      *rightNode->arguments[i])) {
                    return false;
                }
            }

            return true;
        }
    };

    return dispatch::Dispatcher::dispatch(left, EquivalentVisitor{this, right});
}

bool UltraSimplifier::isZero(const MathNode& node) const {
    return dispatch::Dispatcher::dispatch(
        node, [](const auto& concreteNode) -> bool {
            using NodeType = std::decay_t<decltype(concreteNode)>;

            if constexpr (std::is_same_v<NodeType, NumberNode>) {
                return core::Value::parse(concreteNode.value) ==
                       core::Value::parse("0");

            } else {
                return false;
            }
        });
}

bool UltraSimplifier::isOne(const MathNode& node) const {
    return dispatch::Dispatcher::dispatch(
        node, [](const auto& concreteNode) -> bool {
            using NodeType = std::decay_t<decltype(concreteNode)>;

            if constexpr (std::is_same_v<NodeType, NumberNode>) {
                return core::Value::parse(concreteNode.value) ==
                       core::Value::parse("1");

            } else {
                return false;
            }
        });
}

}  // namespace numathap::symbolic
