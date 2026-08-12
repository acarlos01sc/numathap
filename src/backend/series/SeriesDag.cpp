/**
 * @file SeriesDag.cpp
 * @brief Implementation of the DAG used by the series backend.
 */

#include "numathap/backend/series/SeriesDag.hpp"

#include <algorithm>
#include <array>
#include <functional>
#include <limits>
#include <span>
#include <stdexcept>
#include <utility>

#include "numathap/backend/differentiate/CMathDifferentiator.hpp"

namespace numathap::backend::series {

namespace {

using math::BinaryOp;
using math::FunctionNode;
using math::MathNodePtr;
using math::NumberNode;
using math::SymbolNode;
using math::UnaryOp;

std::size_t hashCombine(std::size_t seed, std::size_t value) noexcept {
    constexpr std::size_t magic =
        sizeof(std::size_t) == 8
            ? static_cast<std::size_t>(0x9e3779b97f4a7c15ULL)
            : static_cast<std::size_t>(0x9e3779b9UL);

    seed ^= value + magic + (seed << 6U) + (seed >> 2U);
    return seed;
}

}  // namespace

// ============================================================================
// Construction
// ============================================================================

SeriesDag::SeriesDag(const math::PreparedAst& ast, std::size_t maxNodes)
    : storage_(std::make_shared<Storage>()),
      maxNodes_(maxNodes),
      environment_(ast.environment()) {
    if (ast.empty() || ast.root() == nullptr) {
        return;
    }

    root_ = importNode(*ast.root());
}

// ============================================================================
// NodeKey
// ============================================================================

bool SeriesDag::NodeKey::operator==(const NodeKey& other) const noexcept {
    return kind == other.kind && unaryOp == other.unaryOp &&
           binaryOp == other.binaryOp && value == other.value &&
           children == other.children;
}

std::size_t SeriesDag::NodeKeyHash::operator()(
    const NodeKey& key) const noexcept {
    std::size_t hash =
        std::hash<int>{}(static_cast<int>(key.kind));

    hash = hashCombine(
        hash, std::hash<int>{}(static_cast<int>(key.unaryOp)));

    hash = hashCombine(
        hash, std::hash<int>{}(static_cast<int>(key.binaryOp)));

    hash = hashCombine(
        hash, std::hash<std::string>{}(key.value));

    for (NodeId child : key.children) {
        hash = hashCombine(hash, std::hash<NodeId>{}(child));
    }

    return hash;
}

// ============================================================================
// DAG node creation / hash-consing
// ============================================================================

SeriesDag::NodeId SeriesDag::intern(NodeKey key) {
    const auto existing = storage_->interned.find(key);

    if (existing != storage_->interned.end()) {
        return existing->second;
    }

    checkNodeLimit();

    if (storage_->nodes.size() >=
        static_cast<std::size_t>(std::numeric_limits<NodeId>::max())) {
        throw DagSizeLimitExceeded(
            static_cast<std::size_t>(std::numeric_limits<NodeId>::max()));
    }

    const NodeId id =
        static_cast<NodeId>(storage_->nodes.size());

    auto node = std::make_shared<Node>();

    node->kind = key.kind;
    node->unaryOp = key.unaryOp;
    node->binaryOp = key.binaryOp;
    node->value = std::move(key.value);
    node->children = std::move(key.children);

    storage_->nodes.push_back(
        std::static_pointer_cast<const Node>(node));

    /*
     * The key was moved into the node above, so reconstruct it from
     * the immutable node for insertion into the hash-consing table.
     */
    NodeKey storedKey;

    storedKey.kind = node->kind;
    storedKey.unaryOp = node->unaryOp;
    storedKey.binaryOp = node->binaryOp;
    storedKey.value = node->value;
    storedKey.children = node->children;

    storage_->interned.emplace(std::move(storedKey), id);

    return id;
}

SeriesDag::NodeId SeriesDag::makeNumber(std::string_view value) {
    NodeKey key;

    key.kind = NodeKind::Number;
    key.value = std::string(value);

    return intern(std::move(key));
}

SeriesDag::NodeId SeriesDag::makeNumber(const core::Value& value) {
    return makeNumber(value.str());
}

SeriesDag::NodeId SeriesDag::makeSymbol(std::string_view name) {
    NodeKey key;

    key.kind = NodeKind::Symbol;
    key.value = std::string(name);

    return intern(std::move(key));
}

SeriesDag::NodeId SeriesDag::makeUnary(
    math::UnaryOp op,
    NodeId operand) {
    NodeKey key;

    key.kind = NodeKind::Unary;
    key.unaryOp = op;
    key.children.push_back(operand);

    return intern(std::move(key));
}

SeriesDag::NodeId SeriesDag::makeBinary(
    math::BinaryOp op,
    NodeId left,
    NodeId right) {
    NodeKey key;

    key.kind = NodeKind::Binary;
    key.binaryOp = op;
    key.children.push_back(left);
    key.children.push_back(right);

    return intern(std::move(key));
}

SeriesDag::NodeId SeriesDag::makeFunction(
    std::string_view name,
    const std::vector<NodeId>& arguments) {
    NodeKey key;

    key.kind = NodeKind::Function;
    key.value = std::string(name);
    key.children = arguments;

    return intern(std::move(key));
}

// ============================================================================
// Import from MathNode
// ============================================================================

SeriesDag::NodeId SeriesDag::importNode(
    const math::MathNode& node) {
    if (const auto* number =
            dynamic_cast<const NumberNode*>(&node)) {
        return importNumber(*number);
    }

    if (const auto* symbol =
            dynamic_cast<const SymbolNode*>(&node)) {
        return importSymbol(*symbol);
    }

    if (const auto* unary =
            dynamic_cast<const math::UnaryNode*>(&node)) {
        return importUnary(*unary);
    }

    if (const auto* binary =
            dynamic_cast<const math::BinaryNode*>(&node)) {
        return importBinary(*binary);
    }

    if (const auto* function =
            dynamic_cast<const FunctionNode*>(&node)) {
        return importFunction(*function);
    }

    throw std::runtime_error(
        "SeriesDag: unknown MathNode type.");
}

SeriesDag::NodeId SeriesDag::importNumber(
    const math::NumberNode& node) {
    return makeNumber(node.value);
}

SeriesDag::NodeId SeriesDag::importSymbol(
    const math::SymbolNode& node) {
    return makeSymbol(node.name);
}

SeriesDag::NodeId SeriesDag::importUnary(
    const math::UnaryNode& node) {
    if (!node.operand) {
        throw std::runtime_error(
            "SeriesDag: unary node has no operand.");
    }

    return makeUnary(
        node.op,
        importNode(*node.operand));
}

SeriesDag::NodeId SeriesDag::importBinary(
    const math::BinaryNode& node) {
    if (!node.left || !node.right) {
        throw std::runtime_error(
            "SeriesDag: binary node has a null operand.");
    }

    return makeBinary(
        node.op,
        importNode(*node.left),
        importNode(*node.right));
}

SeriesDag::NodeId SeriesDag::importFunction(
    const math::FunctionNode& node) {
    std::vector<NodeId> arguments;

    arguments.reserve(node.arguments.size());

    for (const auto& argument : node.arguments) {
        if (!argument) {
            throw std::runtime_error(
                "SeriesDag: function node contains a null argument.");
        }

        arguments.push_back(importNode(*argument));
    }

    return makeFunction(node.name, arguments);
}

SeriesDag::NodeId SeriesDag::importMathNode(
    const math::MathNode& node) {
    return importNode(node);
}

// ============================================================================
// Differentiation
// ============================================================================

SeriesDag SeriesDag::differentiate(
    std::string_view variable) const {
    if (root_ == InvalidNode) {
        return *this;
    }

    if (variable.empty()) {
        throw std::invalid_argument(
            "SeriesDag: differentiation variable cannot be empty.");
    }

    /*
     * Storage is intentionally shared between derivative DAGs.
     *
     * The DAG nodes themselves are immutable. The shared storage is extended
     * only by intern(), which preserves structural sharing and hash-consing.
     */
    auto* self = const_cast<SeriesDag*>(this);

    std::unordered_map<NodeId, NodeId> memo;
    memo.reserve(storage_->nodes.size());

    const NodeId derivative =
        self->differentiateNode(root_, variable, memo);

    /*
     * Keep the two local symbolic passes inside SeriesDag.
     */
    const NodeId folded =
        self->constantFold(derivative);

    const NodeId simplified =
        self->simplify(folded);

    SeriesDag result(*this);
    result.root_ = simplified;

    return result;
}

SeriesDag::NodeId SeriesDag::differentiateNode(
    NodeId node,
    std::string_view variable,
    std::unordered_map<NodeId, NodeId>& memo) {
    const auto found = memo.find(node);

    if (found != memo.end()) {
        return found->second;
    }

    if (node == InvalidNode ||
        node >= storage_->nodes.size()) {
        throw std::runtime_error(
            "SeriesDag: invalid node during differentiation.");
    }

    const Node& current = *storage_->nodes[node];

    NodeId result = InvalidNode;

    switch (current.kind) {
        case NodeKind::Number:
            result = differentiateNumber(node);
            break;

        case NodeKind::Symbol:
            result = differentiateSymbol(node, variable);
            break;

        case NodeKind::Unary:
            result = differentiateUnary(
                node, variable, memo);
            break;

        case NodeKind::Binary:
            result = differentiateBinary(
                node, variable, memo);
            break;

        case NodeKind::Function:
            result = differentiateFunction(
                node, variable, memo);
            break;
    }

    memo.emplace(node, result);

    return result;
}

SeriesDag::NodeId SeriesDag::differentiateNumber(
    NodeId) {
    return makeNumber("0");
}

SeriesDag::NodeId SeriesDag::differentiateSymbol(
    NodeId node,
    std::string_view variable) {
    const Node& current = *storage_->nodes[node];

    if (current.value == variable) {
        return makeNumber("1");
    }

    return makeNumber("0");
}

SeriesDag::NodeId SeriesDag::differentiateUnary(
    NodeId node,
    std::string_view variable,
    std::unordered_map<NodeId, NodeId>& memo) {
    const Node& current = *storage_->nodes[node];

    const NodeId derivative =
        differentiateNode(
            current.children[0],
            variable,
            memo);

    switch (current.unaryOp) {
        case UnaryOp::Plus:
            return derivative;

        case UnaryOp::Minus:
            return makeUnary(
                UnaryOp::Minus,
                derivative);
    }

    throw std::runtime_error(
        "SeriesDag: unsupported unary operator.");
}

SeriesDag::NodeId SeriesDag::differentiateBinary(
    NodeId node,
    std::string_view variable,
    std::unordered_map<NodeId, NodeId>& memo) {
    const Node& current = *storage_->nodes[node];

    const NodeId left =
        current.children[0];

    const NodeId right =
        current.children[1];

    const NodeId leftDerivative =
        differentiateNode(
            left,
            variable,
            memo);

    const NodeId rightDerivative =
        differentiateNode(
            right,
            variable,
            memo);

    switch (current.binaryOp) {
        case BinaryOp::Add:
            return makeBinary(
                BinaryOp::Add,
                leftDerivative,
                rightDerivative);

        case BinaryOp::Subtract:
            return makeBinary(
                BinaryOp::Subtract,
                leftDerivative,
                rightDerivative);

        case BinaryOp::Multiply: {
            /*
             * (u v)' = u'v + uv'
             */
            const NodeId first =
                makeBinary(
                    BinaryOp::Multiply,
                    leftDerivative,
                    right);

            const NodeId second =
                makeBinary(
                    BinaryOp::Multiply,
                    left,
                    rightDerivative);

            return makeBinary(
                BinaryOp::Add,
                first,
                second);
        }

        case BinaryOp::Divide: {
            /*
             * (u/v)' = (u'v - uv') / v^2
             */
            const NodeId first =
                makeBinary(
                    BinaryOp::Multiply,
                    leftDerivative,
                    right);

            const NodeId second =
                makeBinary(
                    BinaryOp::Multiply,
                    left,
                    rightDerivative);

            const NodeId numerator =
                makeBinary(
                    BinaryOp::Subtract,
                    first,
                    second);

            const NodeId denominator =
                makeBinary(
                    BinaryOp::Power,
                    right,
                    makeNumber("2"));

            return makeBinary(
                BinaryOp::Divide,
                numerator,
                denominator);
        }

        case BinaryOp::Power:
            return differentiatePower(
                node,
                variable,
                memo);
    }

    throw std::runtime_error(
        "SeriesDag: unsupported binary operator.");
}

SeriesDag::NodeId SeriesDag::differentiateFunction(
    NodeId node,
    std::string_view variable,
    std::unordered_map<NodeId, NodeId>& memo) {
    const Node& current = *storage_->nodes[node];

    if (current.children.size() != 1) {
        throw std::invalid_argument(
            "SeriesDag: function '" + current.value +
            "' differentiation requires exactly one argument.");
    }

    const NodeId argument =
        current.children[0];

    const NodeId argumentDerivative =
        differentiateNode(
            argument,
            variable,
            memo);

    return differentiateWithMathLibrary(
        node,
        argumentDerivative);
}

SeriesDag::NodeId SeriesDag::differentiatePower(
    NodeId node,
    std::string_view variable,
    std::unordered_map<NodeId, NodeId>& memo) {
    const Node& current = *storage_->nodes[node];

    const NodeId base =
        current.children[0];

    const NodeId exponent =
        current.children[1];

    const NodeId baseDerivative =
        differentiateNode(
            base,
            variable,
            memo);

    const NodeId exponentDerivative =
        differentiateNode(
            exponent,
            variable,
            memo);

    return differentiatePowerWithMathLibrary(
        node,
        baseDerivative,
        exponentDerivative);
}

SeriesDag::NodeId SeriesDag::differentiateWithMathLibrary(
    NodeId node,
    NodeId argumentDerivative) {
    auto functionNode =
        buildMathNode(node);

    auto* function =
        dynamic_cast<FunctionNode*>(functionNode.get());

    if (function == nullptr) {
        throw std::runtime_error(
            "SeriesDag: internal function conversion failed.");
    }

    auto derivativeArgument =
        buildMathNode(argumentDerivative);

    differentiate::CMathDifferentiator differentiator;

    MathNodePtr result =
        differentiator.differentiate(
            *function,
            std::move(derivativeArgument));

    if (!result) {
        throw std::runtime_error(
            "SeriesDag: CMathDifferentiator returned a null node.");
    }

    return importMathNode(*result);
}

SeriesDag::NodeId
SeriesDag::differentiatePowerWithMathLibrary(
    NodeId node,
    NodeId baseDerivative,
    NodeId exponentDerivative) {
    auto powerNode =
        buildMathNode(node);

    auto* binary =
        dynamic_cast<math::BinaryNode*>(powerNode.get());

    if (binary == nullptr) {
        throw std::runtime_error(
            "SeriesDag: internal power conversion failed.");
    }

    auto leftDerivative =
        buildMathNode(baseDerivative);

    auto rightDerivative =
        buildMathNode(exponentDerivative);

    differentiate::CMathDifferentiator differentiator;

    MathNodePtr result =
        differentiator.differentiate(
            *binary,
            std::move(leftDerivative),
            std::move(rightDerivative));

    if (!result) {
        throw std::runtime_error(
            "SeriesDag: CMathDifferentiator returned a null node.");
    }

    return importMathNode(*result);
}

// ============================================================================
// Constant folding
// ============================================================================

SeriesDag::NodeId SeriesDag::constantFold(NodeId node) {
    if (node == InvalidNode) {
        return node;
    }

    const Node& current =
        *storage_->nodes[node];

    switch (current.kind) {
        case NodeKind::Number:
        case NodeKind::Symbol:
            return node;

        case NodeKind::Unary:
            return constantFoldUnary(
                current.unaryOp,
                constantFold(current.children[0]));

        case NodeKind::Binary:
            return constantFoldBinary(
                current.binaryOp,
                constantFold(current.children[0]),
                constantFold(current.children[1]));

        case NodeKind::Function: {
            std::vector<NodeId> arguments;

            arguments.reserve(
                current.children.size());

            bool allNumbers = true;

            for (NodeId child : current.children) {
                NodeId folded =
                    constantFold(child);

                arguments.push_back(folded);

                if (!isNumber(folded)) {
                    allNumbers = false;
                }
            }

            NodeId result =
                makeFunction(
                    current.value,
                    arguments);

            if (!allNumbers) {
                return result;
            }

            std::vector<core::Value> values;

            values.reserve(arguments.size());

            for (NodeId argument : arguments) {
                values.push_back(
                    numberValue(argument));
            }

            const core::Value value =
                environment_.mathAdapter().callFunction(
                    current.value,
                    std::span<const core::Value>(
                        values.data(),
                        values.size()));

            return makeNumber(value);
        }
    }

    throw std::runtime_error(
        "SeriesDag: unsupported node during constant folding.");
}

SeriesDag::NodeId SeriesDag::constantFoldUnary(
    math::UnaryOp op,
    NodeId operand) {
    if (!isNumber(operand)) {
        return makeUnary(op, operand);
    }

    const core::Value value =
        numberValue(operand);

    switch (op) {
        case UnaryOp::Plus:
            return makeNumber(+value);

        case UnaryOp::Minus:
            return makeNumber(-value);
    }

    throw std::runtime_error(
        "SeriesDag: unsupported unary operator during constant folding.");
}

SeriesDag::NodeId SeriesDag::constantFoldBinary(
    math::BinaryOp op,
    NodeId left,
    NodeId right) {
    if (!isNumber(left) || !isNumber(right)) {
        return makeBinary(op, left, right);
    }

    const core::Value lhs =
        numberValue(left);

    const core::Value rhs =
        numberValue(right);

    switch (op) {
        case BinaryOp::Add:
            return makeNumber(lhs + rhs);

        case BinaryOp::Subtract:
            return makeNumber(lhs - rhs);

        case BinaryOp::Multiply:
            return makeNumber(lhs * rhs);

        case BinaryOp::Divide:
            return makeNumber(lhs / rhs);

        case BinaryOp::Power: {
            const std::array<core::Value, 2> arguments{
                lhs, rhs};

            return makeNumber(
                environment_.mathAdapter().callFunction(
                    "pow",
                    std::span<const core::Value>(
                        arguments.data(),
                        arguments.size())));
        }
    }

    throw std::runtime_error(
        "SeriesDag: unsupported binary operator during constant folding.");
}

bool SeriesDag::isNumber(NodeId node) const noexcept {
    return node != InvalidNode &&
           node < storage_->nodes.size() &&
           storage_->nodes[node]->kind ==
               NodeKind::Number;
}

core::Value SeriesDag::numberValue(NodeId node) const {
    if (!isNumber(node)) {
        throw std::invalid_argument(
            "SeriesDag: node is not a number.");
    }

    return core::Value::parse(
        storage_->nodes[node]->value);
}

// ============================================================================
// Simplification
// ============================================================================

SeriesDag::NodeId SeriesDag::simplify(NodeId node) {
    if (node == InvalidNode) {
        return node;
    }

    const Node& current =
        *storage_->nodes[node];

    switch (current.kind) {
        case NodeKind::Number:
        case NodeKind::Symbol:
            return node;

        case NodeKind::Unary:
            return simplifyUnary(
                current.unaryOp,
                simplify(current.children[0]));

        case NodeKind::Binary:
            return simplifyBinary(
                current.binaryOp,
                simplify(current.children[0]),
                simplify(current.children[1]));

        case NodeKind::Function:
            return simplifyFunction(node);
    }

    throw std::runtime_error(
        "SeriesDag: unsupported node during simplification.");
}

SeriesDag::NodeId SeriesDag::simplifyUnary(
    math::UnaryOp op,
    NodeId operand) {
    if (op == UnaryOp::Plus) {
        return operand;
    }

    if (op == UnaryOp::Minus &&
        operand != InvalidNode) {
        const Node& child =
            *storage_->nodes[operand];

        if (child.kind == NodeKind::Unary &&
            child.unaryOp == UnaryOp::Minus &&
            child.children.size() == 1) {
            return child.children[0];
        }
    }

    return makeUnary(op, operand);
}

SeriesDag::NodeId SeriesDag::simplifyBinary(
    math::BinaryOp op,
    NodeId left,
    NodeId right) {
    switch (op) {
        case BinaryOp::Add:
            if (isZero(left)) {
                return right;
            }

            if (isZero(right)) {
                return left;
            }

            if (isZero(left) &&
                isZero(right)) {
                return makeNumber("0");
            }

            if (right != InvalidNode) {
                const Node& r =
                    *storage_->nodes[right];

                if (r.kind == NodeKind::Unary &&
                    r.unaryOp == UnaryOp::Minus &&
                    r.children.size() == 1 &&
                    equivalent(left, r.children[0])) {
                    return makeNumber("0");
                }
            }

            if (left != InvalidNode) {
                const Node& l =
                    *storage_->nodes[left];

                if (l.kind == NodeKind::Unary &&
                    l.unaryOp == UnaryOp::Minus &&
                    l.children.size() == 1 &&
                    equivalent(l.children[0], right)) {
                    return makeNumber("0");
                }
            }

            break;

        case BinaryOp::Subtract:
            if (equivalent(left, right)) {
                return makeNumber("0");
            }

            if (isZero(right)) {
                return left;
            }

            if (isZero(left)) {
                return makeUnary(
                    UnaryOp::Minus,
                    right);
            }

            break;

        case BinaryOp::Multiply:
            if (isZero(left)) {
                return left;
            }

            if (isZero(right)) {
                return right;
            }

            if (isOne(left)) {
                return right;
            }

            if (isOne(right)) {
                return left;
            }

            if (left != InvalidNode) {
                const Node& l =
                    *storage_->nodes[left];

                if (l.kind == NodeKind::Unary &&
                    l.unaryOp == UnaryOp::Minus &&
                    l.children.size() == 1 &&
                    isOne(l.children[0])) {
                    return makeUnary(
                        UnaryOp::Minus,
                        right);
                }
            }

            if (right != InvalidNode) {
                const Node& r =
                    *storage_->nodes[right];

                if (r.kind == NodeKind::Unary &&
                    r.unaryOp == UnaryOp::Minus &&
                    r.children.size() == 1 &&
                    isOne(r.children[0])) {
                    return makeUnary(
                        UnaryOp::Minus,
                        left);
                }
            }

            break;

        case BinaryOp::Divide:
            if (isZero(left)) {
                return left;
            }

            if (isOne(right)) {
                return left;
            }

            break;

        case BinaryOp::Power:
            if (isOne(right)) {
                return left;
            }

            break;
    }

    return makeBinary(
        op,
        left,
        right);
}

SeriesDag::NodeId SeriesDag::simplifyFunction(
    NodeId node) {
    const Node& current =
        *storage_->nodes[node];

    std::vector<NodeId> arguments;

    arguments.reserve(
        current.children.size());

    for (NodeId child :
         current.children) {
        arguments.push_back(
            simplify(child));
    }

    return makeFunction(
        current.value,
        arguments);
}

bool SeriesDag::isZero(NodeId node) const {
    if (!isNumber(node)) {
        return false;
    }

    return numberValue(node) ==
           core::Value::parse("0");
}

bool SeriesDag::isOne(NodeId node) const {
    if (!isNumber(node)) {
        return false;
    }

    return numberValue(node) ==
           core::Value::parse("1");
}

bool SeriesDag::equivalent(
    NodeId left,
    NodeId right) const noexcept {
    return left == right;
}

// ============================================================================
// Evaluation
// ============================================================================

core::Value SeriesDag::evaluate(
    const core::Context& context) const {
    if (root_ == InvalidNode) {
        throw std::runtime_error(
            "SeriesDag: cannot evaluate an empty DAG.");
    }

    std::unordered_map<NodeId, core::Value> memo;

    memo.reserve(
        storage_->nodes.size());

    return evaluateNode(
        root_,
        context,
        memo);
}

SeriesDag::DerivativeValues
SeriesDag::evaluateDerivativesAt(
    std::string_view variable,
    const core::Value& center,
    std::size_t order) const {
    if (root_ == InvalidNode) {
        throw std::runtime_error(
            "SeriesDag: cannot evaluate derivatives of an empty DAG.");
    }

    if (variable.empty()) {
        throw std::invalid_argument(
            "SeriesDag: differentiation variable cannot be empty.");
    }

    /*
     * The public interface of evaluateDerivativesAt() deliberately receives
     * only the variable and the center. Therefore the evaluation context
     * used here contains the center assignment:
     *
     *     variable = center
     *
     * Constants such as pi and e continue to be resolved through the
     * MathEnvironment.
     */
    core::Context context;

    context.setValue(
        std::string(variable),
        center.str());

    DerivativeValues values;

    values.reserve(order + 1);

    /*
     * d^0 f / dx^0 = f
     */
    SeriesDag current(*this);

    values.push_back(
        current.evaluate(context));

    /*
     * Successively construct
     *
     *     f'
     *     f''
     *     f'''
     *     ...
     *
     * and evaluate each derivative at the same center.
     *
     * This deliberately reuses the existing symbolic differentiation
     * mechanism. No second set of differentiation rules is introduced.
     */
    for (std::size_t n = 1; n <= order; ++n) {
        current = current.differentiate(variable);

        values.push_back(
            current.evaluate(context));
    }

    return values;
}

core::Value SeriesDag::evaluateNode(
    NodeId node,
    const core::Context& context,
    std::unordered_map<NodeId, core::Value>& memo) const {
    const auto found =
        memo.find(node);

    if (found != memo.end()) {
        return found->second;
    }

    if (node == InvalidNode ||
        node >= storage_->nodes.size()) {
        throw std::runtime_error(
            "SeriesDag: invalid node during evaluation.");
    }

    const Node& current =
        *storage_->nodes[node];

    core::Value result;

    switch (current.kind) {
        case NodeKind::Number:
            result = evaluateNumber(node);
            break;

        case NodeKind::Symbol:
            result = evaluateSymbol(
                node,
                context);
            break;

        case NodeKind::Unary:
            result = evaluateUnary(
                node,
                context,
                memo);
            break;

        case NodeKind::Binary:
            result = evaluateBinary(
                node,
                context,
                memo);
            break;

        case NodeKind::Function:
            result = evaluateFunction(
                node,
                context,
                memo);
            break;
    }

    memo.emplace(
        node,
        result);

    return result;
}

core::Value SeriesDag::evaluateNumber(
    NodeId node) const {
    return core::Value::parse(
        storage_->nodes[node]->value);
}

core::Value SeriesDag::evaluateSymbol(
    NodeId node,
    const core::Context& context) const {
    const std::string& name =
        storage_->nodes[node]->value;

    if (context.hasValue(name)) {
        const std::string resolved =
            context.resolveValue(name);

        try {
            return core::Value::parse(
                resolved);
        } catch (const std::exception&) {
            throw std::invalid_argument(
                "SeriesDag: context value for symbol '" +
                name +
                "' is not a numeric value: '" +
                resolved +
                "'.");
        }
    }

    return environment_.mathAdapter()
        .resolveConstant(name);
}

core::Value SeriesDag::evaluateUnary(
    NodeId node,
    const core::Context& context,
    std::unordered_map<NodeId, core::Value>& memo) const {
    const Node& current =
        *storage_->nodes[node];

    const core::Value operand =
        evaluateNode(
            current.children[0],
            context,
            memo);

    switch (current.unaryOp) {
        case UnaryOp::Plus:
            return +operand;

        case UnaryOp::Minus:
            return -operand;
    }

    throw std::runtime_error(
        "SeriesDag: unsupported unary operator during evaluation.");
}

core::Value SeriesDag::evaluateBinary(
    NodeId node,
    const core::Context& context,
    std::unordered_map<NodeId, core::Value>& memo) const {
    const Node& current =
        *storage_->nodes[node];

    const core::Value left =
        evaluateNode(
            current.children[0],
            context,
            memo);

    const core::Value right =
        evaluateNode(
            current.children[1],
            context,
            memo);

    switch (current.binaryOp) {
        case BinaryOp::Add:
            return left + right;

        case BinaryOp::Subtract:
            return left - right;

        case BinaryOp::Multiply:
            return left * right;

        case BinaryOp::Divide:
            return left / right;

        case BinaryOp::Power: {
            const std::array<core::Value, 2> arguments{
                left,
                right};

            return environment_.mathAdapter()
                .callFunction(
                    "pow",
                    std::span<const core::Value>(
                        arguments.data(),
                        arguments.size()));
        }
    }

    throw std::runtime_error(
        "SeriesDag: unsupported binary operator during evaluation.");
}

core::Value SeriesDag::evaluateFunction(
    NodeId node,
    const core::Context& context,
    std::unordered_map<NodeId, core::Value>& memo) const {
    const Node& current =
        *storage_->nodes[node];

    std::vector<core::Value> arguments;

    arguments.reserve(
        current.children.size());

    for (NodeId child :
         current.children) {
        arguments.push_back(
            evaluateNode(
                child,
                context,
                memo));
    }

    return environment_.mathAdapter()
        .callFunction(
            current.value,
            std::span<const core::Value>(
                arguments.data(),
                arguments.size()));
}

// ============================================================================
// Conversion to MathNode
// ============================================================================

math::MathNodePtr SeriesDag::buildMathNode(
    NodeId node) const {
    if (node == InvalidNode ||
        node >= storage_->nodes.size()) {
        throw std::runtime_error(
            "SeriesDag: invalid node during MathNode conversion.");
    }

    const Node& current =
        *storage_->nodes[node];

    switch (current.kind) {
        case NodeKind::Number:
            return std::make_unique<NumberNode>(
                current.value);

        case NodeKind::Symbol:
            return std::make_unique<SymbolNode>(
                current.value);

        case NodeKind::Unary:
            if (current.children.size() != 1) {
                throw std::runtime_error(
                    "SeriesDag: invalid unary node.");
            }

            return std::make_unique<math::UnaryNode>(
                current.unaryOp,
                buildMathNode(
                    current.children[0]));

        case NodeKind::Binary:
            if (current.children.size() != 2) {
                throw std::runtime_error(
                    "SeriesDag: invalid binary node.");
            }

            return std::make_unique<math::BinaryNode>(
                current.binaryOp,
                buildMathNode(
                    current.children[0]),
                buildMathNode(
                    current.children[1]));

        case NodeKind::Function: {
            std::vector<MathNodePtr> arguments;

            arguments.reserve(
                current.children.size());

            for (NodeId child :
                 current.children) {
                arguments.push_back(
                    buildMathNode(child));
            }

            return std::make_unique<FunctionNode>(
                current.value,
                std::move(arguments));
        }
    }

    throw std::runtime_error(
        "SeriesDag: unsupported node during MathNode conversion.");
}

math::MathNodePtr SeriesDag::toMathNode() const {
    if (root_ == InvalidNode) {
        return nullptr;
    }

    return buildMathNode(root_);
}

math::MathNodePtr SeriesDag::toMathNode(
    NodeId node) const {
    return buildMathNode(node);
}

// ============================================================================
// DAG printing
// ============================================================================

void SeriesDag::print(
    std::ostream& os) const {
    os << "SeriesDag\n";

    if (root_ == InvalidNode) {
        os << "Root: <invalid>\n";
        os << "Nodes: 0\n";
        return;
    }

    os << "Root: [" << root_ << "]\n";
    os << "Nodes: " << nodeCount() << "\n\n";

    std::vector<bool> visited(
        storage_->nodes.size(),
        false);

    std::vector<NodeId> stack;

    stack.push_back(root_);

    /*
     * Print reachable nodes in NodeId order.
     *
     * Since NodeId is assigned by intern(), this also makes it easier
     * to compare the printed DAG with the internal storage.
     */
    std::vector<NodeId> nodes;

    nodes.reserve(
        nodeCount());

    while (!stack.empty()) {
        const NodeId node =
            stack.back();

        stack.pop_back();

        if (node == InvalidNode ||
            node >= storage_->nodes.size() ||
            visited[node]) {
            continue;
        }

        visited[node] = true;
        nodes.push_back(node);

        const Node& current =
            *storage_->nodes[node];

        for (auto it =
                 current.children.rbegin();
             it != current.children.rend();
             ++it) {
            stack.push_back(*it);
        }
    }

    std::sort(
        nodes.begin(),
        nodes.end());

    for (NodeId node :
         nodes) {
        printNode(node, os);
        os << '\n';
    }
}

void SeriesDag::printNode(
    NodeId node,
    std::ostream& os) const {
    if (node == InvalidNode ||
        node >= storage_->nodes.size()) {
        os << "[invalid]";
        return;
    }

    const Node& current =
        *storage_->nodes[node];

    os << '[' << node << "] ";

    switch (current.kind) {
        case NodeKind::Number:
            os << "Number("
               << current.value
               << ')';
            break;

        case NodeKind::Symbol:
            os << "Symbol("
               << current.value
               << ')';
            break;

        case NodeKind::Unary:
            os << "Unary("
               << unaryOpName(
                      current.unaryOp)
               << ')';

            if (!current.children.empty()) {
                os << "\n"
                   << "     └── ["
                   << current.children[0]
                   << ']';
            }

            break;

        case NodeKind::Binary:
            os << "Binary("
               << binaryOpName(
                      current.binaryOp)
               << ')';

            if (current.children.size() >= 2) {
                os << "\n"
                   << "     ├── ["
                   << current.children[0]
                   << "]\n"
                   << "     └── ["
                   << current.children[1]
                   << ']';
            }

            break;

        case NodeKind::Function:
            os << "Function("
               << current.value
               << ')';

            for (std::size_t i = 0;
                 i < current.children.size();
                 ++i) {
                const bool last =
                    i + 1 ==
                    current.children.size();

                os << "\n"
                   << "     "
                   << (last ? "└── "
                            : "├── ")
                   << '['
                   << current.children[i]
                   << ']';
            }

            break;
    }

    os << '\n';
}

const char* SeriesDag::nodeKindName(
    NodeKind kind) noexcept {
    switch (kind) {
        case NodeKind::Number:
            return "Number";

        case NodeKind::Symbol:
            return "Symbol";

        case NodeKind::Unary:
            return "Unary";

        case NodeKind::Binary:
            return "Binary";

        case NodeKind::Function:
            return "Function";
    }

    return "Unknown";
}

const char* SeriesDag::unaryOpName(
    math::UnaryOp op) noexcept {
    switch (op) {
        case math::UnaryOp::Plus:
            return "+";

        case math::UnaryOp::Minus:
            return "-";
    }

    return "?";
}

const char* SeriesDag::binaryOpName(
    math::BinaryOp op) noexcept {
    switch (op) {
        case math::BinaryOp::Add:
            return "+";

        case math::BinaryOp::Subtract:
            return "-";

        case math::BinaryOp::Multiply:
            return "*";

        case BinaryOp::Divide:
            return "/";

        case BinaryOp::Power:
            return "^";
    }

    return "?";
}

// ============================================================================
// Statistics
// ============================================================================

std::size_t SeriesDag::nodeCount() const noexcept {
    return countReachableNodes();
}

std::size_t SeriesDag::countReachableNodes() const noexcept {
    if (root_ == InvalidNode) {
        return 0;
    }

    std::vector<bool> visited(
        storage_->nodes.size(),
        false);

    std::vector<NodeId> stack;

    stack.push_back(root_);

    std::size_t count = 0;

    while (!stack.empty()) {
        const NodeId node =
            stack.back();

        stack.pop_back();

        if (node == InvalidNode ||
            node >= storage_->nodes.size() ||
            visited[node]) {
            continue;
        }

        visited[node] = true;
        ++count;

        const Node& current =
            *storage_->nodes[node];

        for (NodeId child :
             current.children) {
            stack.push_back(child);
        }
    }

    return count;
}

// ============================================================================
// Node limit
// ============================================================================

void SeriesDag::checkNodeLimit() const {
    if (maxNodes_ == 0) {
        return;
    }

    if (storage_->nodes.size() >= maxNodes_) {
        throw DagSizeLimitExceeded(
            maxNodes_);
    }
}

std::size_t SeriesDag::storageNodeCount() const noexcept {
    return storage_->nodes.size();
}

}  // namespace numathap::backend::series