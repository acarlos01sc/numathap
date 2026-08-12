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
 *
 * SeriesDag does not modify the original MathNode tree.
 *
 * Nodes are immutable and structurally shared. Equal expressions are
 * represented by the same DAG node whenever possible.
 *
 * Symbolic differentiation is implemented directly on the DAG. Generic
 * arithmetic differentiation rules are handled here, while mathematical
 * function and general power differentiation rules are delegated to
 * CMathDifferentiator.
 *
 * Evaluation never accesses a concrete mathematical library directly.
 * Mathematical functions and constants are evaluated through the
 * MathAdapter exposed by MathEnvironment.
 */
class SeriesDag {
   public:
    using NodeId = std::uint32_t;

    static constexpr NodeId InvalidNode = static_cast<NodeId>(-1);

    /**
     * @brief Constructs a DAG from a PreparedAst.
     *
     * @param ast Source prepared AST.
     * @param maxNodes Maximum number of nodes allowed in the DAG.
     *                  Zero means unlimited.
     *
     * @throws DagSizeLimitExceeded if the initial DAG exceeds maxNodes.
     */
    explicit SeriesDag(const math::PreparedAst& ast, std::size_t maxNodes = 0);

    SeriesDag(const SeriesDag&) = default;
    SeriesDag(SeriesDag&&) noexcept = default;

    SeriesDag& operator=(const SeriesDag&) = default;
    SeriesDag& operator=(SeriesDag&&) noexcept = default;

    ~SeriesDag() = default;

    /**
     * @brief Calculates the symbolic derivative of this DAG.
     *
     * The current DAG is never structurally modified. Existing nodes are
     * reused whenever possible and derivative results are memoized during
     * the operation.
     *
     * @param variable Differentiation variable.
     *
     * @return A new SeriesDag representing the derivative.
     *
     * @throws DagSizeLimitExceeded if the derivative exceeds maxNodes().
     */
    [[nodiscard]]
    SeriesDag differentiate(std::string_view variable) const;

    /**
     * @brief Evaluates the expression represented by the DAG.
     *
     * Evaluation is memoized for the duration of this call, so a shared
     * node is evaluated only once.
     *
     * Mathematical functions and constants are evaluated through the
     * MathAdapter associated with this DAG's MathEnvironment.
     *
     * @param context Evaluation context.
     *
     * @return Evaluated expression.
     */
    [[nodiscard]]
    core::Value evaluate(const core::Context& context) const;

    /**
     * @brief Converts the DAG back into a MathNode tree.
     *
     * This is intended for interoperability with existing numathap
     * components and diagnostics.
     */
    [[nodiscard]]
    math::MathNodePtr toMathNode() const;

    /**
     * @brief Returns the number of distinct nodes reachable from the root.
     */
    [[nodiscard]]
    std::size_t nodeCount() const noexcept;

    /**
     * @brief Returns the configured maximum DAG size.
     *
     * Zero means unlimited.
     */
    [[nodiscard]]
    std::size_t maxNodes() const noexcept {
        return maxNodes_;
    }

    /**
     * @brief Returns whether the DAG has no root.
     */
    [[nodiscard]]
    bool empty() const noexcept {
        return root_ == InvalidNode;
    }

    /**
     * @brief Returns the environment associated with this DAG.
     */
    [[nodiscard]]
    const config::MathEnvironment& environment() const noexcept {
        return environment_;
    }

    /**
     * @brief Prints the DAG structure.
     *
     * Each distinct node is printed once together with its NodeId.
     * Child references use NodeId values, making structural sharing explicit.
     */
    void print(std::ostream& os) const;

   private:
    // ---------------------------------------------------------------------
    // Internal representation
    // ---------------------------------------------------------------------

    enum class NodeKind { Number, Symbol, Unary, Binary, Function };

    struct Node {
        NodeKind kind{NodeKind::Number};

        math::UnaryOp unaryOp{math::UnaryOp::Plus};
        math::BinaryOp binaryOp{math::BinaryOp::Add};

        /*
         * Number:
         *     textual numeric representation.
         *
         * Symbol:
         *     symbol name.
         *
         * Function:
         *     function name.
         */
        std::string value;

        /*
         * Unary:
         *     children[0] = operand
         *
         * Binary:
         *     children[0] = left
         *     children[1] = right
         *
         * Function:
         *     children = arguments
         */
        std::vector<NodeId> children;
    };

    /**
     * @brief Structural key used for hash-consing.
     */
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

    /**
     * @brief Shared node storage.
     *
     * Nodes are immutable after insertion.
     *
     * The storage is shared between successive derivative DAGs so that
     * unchanged nodes are not copied.
     */
    struct Storage {
        std::vector<std::shared_ptr<const Node>> nodes;

        std::unordered_map<NodeKey, NodeId, NodeKeyHash> interned;
    };

    std::shared_ptr<Storage> storage_;

    NodeId root_{InvalidNode};

    std::size_t maxNodes_{0};

    /*
     * MathEnvironment owns the selected MathAdapter. It is copied when a
     * derivative DAG is created, preserving the mathematical backend and
     * numeric configuration of the source DAG.
     */
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
    NodeId makeUnary(math::UnaryOp op, NodeId operand);

    [[nodiscard]]
    NodeId makeBinary(math::BinaryOp op, NodeId left, NodeId right);

    [[nodiscard]]
    NodeId makeFunction(std::string_view name,
                        const std::vector<NodeId>& arguments);

    /**
     * @brief Returns an existing structurally identical node or creates one.
     */
    [[nodiscard]]
    NodeId intern(NodeKey key);

    // ---------------------------------------------------------------------
    // Differentiation
    // ---------------------------------------------------------------------

    /**
     * @brief Differentiates a DAG node.
     *
     * The memo table is local to one differentiate() operation.
     */
    [[nodiscard]]
    NodeId differentiateNode(NodeId node, std::string_view variable,
                             std::unordered_map<NodeId, NodeId>& memo);

    [[nodiscard]]
    NodeId differentiateNumber(NodeId node);

    [[nodiscard]]
    NodeId differentiateSymbol(NodeId node, std::string_view variable);

    [[nodiscard]]
    NodeId differentiateUnary(NodeId node, std::string_view variable,
                              std::unordered_map<NodeId, NodeId>& memo);

    [[nodiscard]]
    NodeId differentiateBinary(NodeId node, std::string_view variable,
                               std::unordered_map<NodeId, NodeId>& memo);

    [[nodiscard]]
    NodeId differentiateFunction(NodeId node, std::string_view variable,
                                 std::unordered_map<NodeId, NodeId>& memo);

    [[nodiscard]]
    NodeId differentiatePower(NodeId node, std::string_view variable,
                              std::unordered_map<NodeId, NodeId>& memo);

    /**
     * @brief Delegates a function-specific derivative rule to
     * CMathDifferentiator.
     *
     * The derivative of the function argument is calculated directly by
     * SeriesDag. Only the function-specific mathematical rule is delegated.
     */
    [[nodiscard]]
    NodeId differentiateWithMathLibrary(NodeId node, NodeId argumentDerivative);

    /**
     * @brief Delegates the general power rule to CMathDifferentiator.
     *
     * The derivatives of the base and exponent are calculated directly by
     * SeriesDag.
     */
    [[nodiscard]]
    NodeId differentiatePowerWithMathLibrary(NodeId node, NodeId baseDerivative,
                                             NodeId exponentDerivative);

    // ---------------------------------------------------------------------
    // Constant folding
    // ---------------------------------------------------------------------

    /**
     * @brief Performs constant folding on a DAG node.
     *
     * This implementation is deliberately local to SeriesDag and does not
     * depend on symbolic::ConstantFolder.
     */
    [[nodiscard]]
    NodeId constantFold(NodeId node);

    [[nodiscard]]
    NodeId constantFoldUnary(math::UnaryOp op, NodeId operand);

    [[nodiscard]]
    NodeId constantFoldBinary(math::BinaryOp op, NodeId left, NodeId right);

    [[nodiscard]]
    bool isNumber(NodeId node) const noexcept;

    [[nodiscard]]
    core::Value numberValue(NodeId node) const;

    // ---------------------------------------------------------------------
    // Simplification
    // ---------------------------------------------------------------------

    /**
     * @brief Performs local algebraic simplification.
     *
     * This implementation is deliberately local to SeriesDag and does not
     * depend on symbolic::Simplifier.
     */
    [[nodiscard]]
    NodeId simplify(NodeId node);

    [[nodiscard]]
    NodeId simplifyUnary(math::UnaryOp op, NodeId operand);

    [[nodiscard]]
    NodeId simplifyBinary(math::BinaryOp op, NodeId left, NodeId right);

    [[nodiscard]]
    NodeId simplifyFunction(NodeId node);

    [[nodiscard]]
    bool isZero(NodeId node) const;

    [[nodiscard]]
    bool isOne(NodeId node) const;

    [[nodiscard]]
    bool equivalent(NodeId left, NodeId right) const noexcept;

    // ---------------------------------------------------------------------
    // Evaluation
    // ---------------------------------------------------------------------

    [[nodiscard]]
    core::Value evaluateNode(
        NodeId node, const core::Context& context,
        std::unordered_map<NodeId, core::Value>& memo) const;

    [[nodiscard]]
    core::Value evaluateNumber(NodeId node) const;

    [[nodiscard]]
    core::Value evaluateSymbol(NodeId node, const core::Context& context) const;

    [[nodiscard]]
    core::Value evaluateUnary(
        NodeId node, const core::Context& context,
        std::unordered_map<NodeId, core::Value>& memo) const;

    [[nodiscard]]
    core::Value evaluateBinary(
        NodeId node, const core::Context& context,
        std::unordered_map<NodeId, core::Value>& memo) const;

    [[nodiscard]]
    core::Value evaluateFunction(
        NodeId node, const core::Context& context,
        std::unordered_map<NodeId, core::Value>& memo) const;

    // ---------------------------------------------------------------------
    // Conversion between DAG and MathNode
    // ---------------------------------------------------------------------

    /**
     * @brief Builds a MathNode representation of a DAG node.
     *
     * Used only at the CMathDifferentiator boundary and by toMathNode().
     */
    [[nodiscard]]
    math::MathNodePtr buildMathNode(NodeId node) const;

    /**
     * @brief Imports a MathNode tree into this DAG.
     */
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
    // Operation-local node limit
    // ---------------------------------------------------------------------

    /**
     * @brief Checks whether creation of a new node is allowed.
     *
     * The node limit applies to distinct nodes introduced into the shared
     * storage, not to references to already existing nodes.
     */
    void checkNodeLimit() const;

    /**
     * @brief Returns the number of nodes currently stored.
     */
    [[nodiscard]]
    std::size_t storageNodeCount() const noexcept;

    void printNode(NodeId node, std::ostream& os) const;

    [[nodiscard]]
    static const char* nodeKindName(NodeKind kind) noexcept;

    [[nodiscard]]
    static const char* unaryOpName(math::UnaryOp op) noexcept;

    [[nodiscard]]
    static const char* binaryOpName(math::BinaryOp op) noexcept;
};

}  // namespace numathap::backend::series