/**
 * @file SeriesDag.hpp
 * @brief DAG representation used by the series backends.
 *
 * SeriesDag provides a hash-consed, structurally shared representation of
 * mathematical expressions. It is intended primarily for symbolic
 * differentiation and Taylor-series generation.
 */
#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "numathap/config/MathEnvironment.hpp"
#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"
#include "numathap/math/MathNode.hpp"
#include "numathap/math/PreparedAst.hpp"

namespace numathap::backend::series {

/**
 * @brief Exception thrown when a SeriesDag exceeds its node limit.
 */
class DagSizeLimitExceeded : public std::runtime_error {
   public:
    explicit DagSizeLimitExceeded(std::size_t maxNodes)
        : std::runtime_error("SeriesDag maximum node count exceeded: " +
                             std::to_string(maxNodes)),
          maxNodes_(maxNodes) {}

    [[nodiscard]]
    std::size_t maxNodes() const noexcept {
        return maxNodes_;
    }

   private:
    std::size_t maxNodes_;
};

/**
 * @brief Directed acyclic graph used internally by the series backend.
 */
class SeriesDag {
   public:
    using NodeId = std::uint32_t;

    static constexpr NodeId InvalidNode = static_cast<NodeId>(-1);

    /**
     * @brief Values of successive derivatives evaluated at a point.
     *
     * values[0] = f(a)
     * values[1] = f'(a)
     * values[2] = f''(a)
     * ...
     * values[n] = f^(n)(a)
     *
     * These are derivative values, not Taylor coefficients.
     */
    using DerivativeValues = std::vector<core::Value>;

    explicit SeriesDag(
        const math::PreparedAst& ast,
        std::size_t maxNodes = 0);

    SeriesDag(const SeriesDag&) = default;
    SeriesDag(SeriesDag&&) noexcept = default;

    SeriesDag& operator=(const SeriesDag&) = default;
    SeriesDag& operator=(SeriesDag&&) noexcept = default;

    ~SeriesDag() = default;

    /**
     * @brief Calculates the symbolic derivative of this DAG.
     */
    [[nodiscard]]
    SeriesDag differentiate(std::string_view variable) const;

    /**
     * @brief Evaluates the expression represented by the DAG.
     */
    [[nodiscard]]
    core::Value evaluate(const core::Context& context) const;

    /**
     * @brief Evaluates successive derivatives at a point using symbolic
     * differentiation.
     *
     * This is the existing implementation and is intentionally preserved.
     */
//    [[nodiscard]]
//    DerivativeValues evaluateDerivativesAt(
//        std::string_view variable,
//        const core::Value& center,
//        std::size_t order) const;

    /**
     * @brief Evaluates successive derivatives directly on the DAG.
     *
     * No symbolic derivative DAGs are constructed.
     *
     * For every DAG node, a vector
     *
     *     [f(a), f'(a), ..., f^(n)(a)]
     *
     * is propagated through the DAG. Multiplication uses the Leibniz rule
     * as a binomial convolution. Power and supported mathematical functions
     * are evaluated through derivative-vector recurrences.
     */
    [[nodiscard]]
    DerivativeValues fastEvaluateDerivativesAt(
        std::string_view variable,
        const core::Value& center,
        std::size_t order) const;

    /**
     * @brief Converts the DAG back into a MathNode tree.
     */
    [[nodiscard]]
    math::MathNodePtr toMathNode() const;

    [[nodiscard]]
    std::size_t nodeCount() const noexcept;

    [[nodiscard]]
    std::size_t maxNodes() const noexcept {
        return maxNodes_;
    }

    [[nodiscard]]
    bool empty() const noexcept {
        return root_ == InvalidNode;
    }

    [[nodiscard]]
    const config::MathEnvironment& environment() const noexcept {
        return environment_;
    }

    void print(std::ostream& os) const;

   private:
    enum class NodeKind {
        Number,
        Symbol,
        Unary,
        Binary,
        Function
    };

    struct Node {
        NodeKind kind{NodeKind::Number};

        math::UnaryOp unaryOp{math::UnaryOp::Plus};
        math::BinaryOp binaryOp{math::BinaryOp::Add};

        std::string value;
        std::vector<NodeId> children;
    };

    struct NodeKey {
        NodeKind kind{NodeKind::Number};

        math::UnaryOp unaryOp{math::UnaryOp::Plus};
        math::BinaryOp binaryOp{math::BinaryOp::Add};

        std::string value;
        std::vector<NodeId> children;

        [[nodiscard]]
        bool operator==(const NodeKey& other) const noexcept;
    };

    struct NodeKeyHash {
        [[nodiscard]]
        std::size_t operator()(const NodeKey& key) const noexcept;
    };

    struct Storage {
        std::vector<std::shared_ptr<const Node>> nodes;
        std::unordered_map<NodeKey, NodeId, NodeKeyHash> interned;
    };

    using DerivativeVector = std::vector<core::Value>;

    std::shared_ptr<Storage> storage_;

    NodeId root_{InvalidNode};

    std::size_t maxNodes_{0};

    config::MathEnvironment environment_;

    // ---------------------------------------------------------------------
    // Import from MathNode
    // ---------------------------------------------------------------------

    [[nodiscard]]
    NodeId importNode(const math::MathNode& node);

    [[nodiscard]]
    NodeId importNumber(const math::NumberNode& node);

    [[nodiscard]]
    NodeId importSymbol(const math::SymbolNode& node);

    [[nodiscard]]
    NodeId importUnary(const math::UnaryNode& node);

    [[nodiscard]]
    NodeId importBinary(const math::BinaryNode& node);

    [[nodiscard]]
    NodeId importFunction(const math::FunctionNode& node);

    // ---------------------------------------------------------------------
    // DAG node construction / hash-consing
    // ---------------------------------------------------------------------

    [[nodiscard]]
    NodeId makeNumber(std::string_view value);

    [[nodiscard]]
    NodeId makeNumber(const core::Value& value);

    [[nodiscard]]
    NodeId makeSymbol(std::string_view name);

    [[nodiscard]]
    NodeId makeUnary(
        math::UnaryOp op,
        NodeId operand);

    [[nodiscard]]
    NodeId makeBinary(
        math::BinaryOp op,
        NodeId left,
        NodeId right);

    [[nodiscard]]
    NodeId makeFunction(
        std::string_view name,
        const std::vector<NodeId>& arguments);

    [[nodiscard]]
    NodeId intern(NodeKey key);

    // ---------------------------------------------------------------------
    // Differentiation
    // ---------------------------------------------------------------------

    [[nodiscard]]
    NodeId differentiateNode(
        NodeId node,
        std::string_view variable,
        std::unordered_map<NodeId, NodeId>& memo);

    [[nodiscard]]
    NodeId differentiateNumber(NodeId node);

    [[nodiscard]]
    NodeId differentiateSymbol(
        NodeId node,
        std::string_view variable);

    [[nodiscard]]
    NodeId differentiateUnary(
        NodeId node,
        std::string_view variable,
        std::unordered_map<NodeId, NodeId>& memo);

    [[nodiscard]]
    NodeId differentiateBinary(
        NodeId node,
        std::string_view variable,
        std::unordered_map<NodeId, NodeId>& memo);

    [[nodiscard]]
    NodeId differentiateFunction(
        NodeId node,
        std::string_view variable,
        std::unordered_map<NodeId, NodeId>& memo);

    [[nodiscard]]
    NodeId differentiatePower(
        NodeId node,
        std::string_view variable,
        std::unordered_map<NodeId, NodeId>& memo);

    [[nodiscard]]
    NodeId differentiateWithMathLibrary(
        NodeId node,
        NodeId argumentDerivative);

    [[nodiscard]]
    NodeId differentiatePowerWithMathLibrary(
        NodeId node,
        NodeId baseDerivative,
        NodeId exponentDerivative);

    // ---------------------------------------------------------------------
    // Constant folding
    // ---------------------------------------------------------------------

    [[nodiscard]]
    NodeId constantFold(NodeId node);

    [[nodiscard]]
    NodeId constantFoldUnary(
        math::UnaryOp op,
        NodeId operand);

    [[nodiscard]]
    NodeId constantFoldBinary(
        math::BinaryOp op,
        NodeId left,
        NodeId right);

    [[nodiscard]]
    bool isNumber(NodeId node) const noexcept;

    [[nodiscard]]
    core::Value numberValue(NodeId node) const;

    // ---------------------------------------------------------------------
    // Simplification
    // ---------------------------------------------------------------------

    [[nodiscard]]
    NodeId simplify(NodeId node);

    [[nodiscard]]
    NodeId simplifyUnary(
        math::UnaryOp op,
        NodeId operand);

    [[nodiscard]]
    NodeId simplifyBinary(
        math::BinaryOp op,
        NodeId left,
        NodeId right);

    [[nodiscard]]
    NodeId simplifyFunction(NodeId node);

    [[nodiscard]]
    bool isZero(NodeId node) const;

    [[nodiscard]]
    bool isOne(NodeId node) const;

    [[nodiscard]]
    bool equivalent(
        NodeId left,
        NodeId right) const noexcept;

    // ---------------------------------------------------------------------
    // Evaluation
    // ---------------------------------------------------------------------

    [[nodiscard]]
    core::Value evaluateNode(
        NodeId node,
        const core::Context& context,
        std::unordered_map<NodeId, core::Value>& memo) const;

    [[nodiscard]]
    core::Value evaluateNumber(NodeId node) const;

    [[nodiscard]]
    core::Value evaluateSymbol(
        NodeId node,
        const core::Context& context) const;

    [[nodiscard]]
    core::Value evaluateUnary(
        NodeId node,
        const core::Context& context,
        std::unordered_map<NodeId, core::Value>& memo) const;

    [[nodiscard]]
    core::Value evaluateBinary(
        NodeId node,
        const core::Context& context,
        std::unordered_map<NodeId, core::Value>& memo) const;

    [[nodiscard]]
    core::Value evaluateFunction(
        NodeId node,
        const core::Context& context,
        std::unordered_map<NodeId, core::Value>& memo) const;

    // ---------------------------------------------------------------------
    // Fast derivative evaluation
    // ---------------------------------------------------------------------

    [[nodiscard]]
    DerivativeVector fastEvaluateNode(
        NodeId node,
        std::string_view variable,
        const core::Context& context,
        std::size_t order,
        std::unordered_map<NodeId, DerivativeVector>& memo) const;

    [[nodiscard]]
    DerivativeVector fastEvaluateNumber(
        NodeId node,
        std::size_t order) const;

    [[nodiscard]]
    DerivativeVector fastEvaluateSymbol(
        NodeId node,
        std::string_view variable,
        const core::Context& context,
        std::size_t order) const;

    [[nodiscard]]
    DerivativeVector fastEvaluateUnary(
        NodeId node,
        const DerivativeVector& operand) const;

    [[nodiscard]]
    DerivativeVector fastEvaluateBinary(
        NodeId node,
        const DerivativeVector& left,
        const DerivativeVector& right) const;

    [[nodiscard]]
    DerivativeVector fastEvaluateFunction(
        NodeId node,
        const DerivativeVector& argument,
        std::size_t order) const;

    [[nodiscard]]
    DerivativeVector fastEvaluatePower(
        NodeId node,
        const DerivativeVector& base,
        const DerivativeVector& exponent,
        std::size_t order) const;

    [[nodiscard]]
    DerivativeVector leibnizProduct(
        const DerivativeVector& left,
        const DerivativeVector& right) const;

    [[nodiscard]]
    DerivativeVector reciprocal(
        const DerivativeVector& value) const;

    [[nodiscard]]
    DerivativeVector divideDerivatives(
        const DerivativeVector& numerator,
        const DerivativeVector& denominator) const;

    [[nodiscard]]
    DerivativeVector logarithm(
        const DerivativeVector& value) const;

    [[nodiscard]]
    DerivativeVector exponential(
        const DerivativeVector& value) const;

    [[nodiscard]]
    DerivativeVector powerConstant(
        const DerivativeVector& base,
        const core::Value& exponent) const;

    [[nodiscard]]
    DerivativeVector powerGeneral(
        const DerivativeVector& base,
        const DerivativeVector& exponent) const;

    [[nodiscard]]
    DerivativeVector sineCosine(
        const DerivativeVector& argument,
        bool sine) const;

    [[nodiscard]]
    DerivativeVector hyperbolicSineCosine(
        const DerivativeVector& argument,
        bool sine) const;

    [[nodiscard]]
    DerivativeVector functionDerivative(
        std::string_view name,
        const DerivativeVector& argument) const;

    [[nodiscard]]
    core::Value evaluateMathFunction(
        std::string_view name,
        const std::vector<core::Value>& arguments) const;

    [[nodiscard]]
    static core::Value binomial(
        std::size_t n,
        std::size_t k);

    // ---------------------------------------------------------------------
    // Conversion between DAG and MathNode
    // ---------------------------------------------------------------------

    [[nodiscard]]
    math::MathNodePtr buildMathNode(NodeId node) const;

    [[nodiscard]]
    NodeId importMathNode(const math::MathNode& node);

    [[nodiscard]]
    math::MathNodePtr toMathNode(NodeId node) const;

    // ---------------------------------------------------------------------
    // DAG statistics
    // ---------------------------------------------------------------------

    [[nodiscard]]
    std::size_t countReachableNodes() const noexcept;

    // ---------------------------------------------------------------------
    // Node limit
    // ---------------------------------------------------------------------

    void checkNodeLimit() const;

    [[nodiscard]]
    std::size_t storageNodeCount() const noexcept;

    void printNode(
        NodeId node,
        std::ostream& os) const;

    [[nodiscard]]
    static const char* nodeKindName(
        NodeKind kind) noexcept;

    [[nodiscard]]
    static const char* unaryOpName(
        math::UnaryOp op) noexcept;

    [[nodiscard]]
    static const char* binaryOpName(
        math::BinaryOp op) noexcept;
};

}  // namespace numathap::backend::series