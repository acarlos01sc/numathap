#pragma once

#include <memory>
#include <string>
#include <vector>

namespace numathap::ast {

/**
 * @brief Base class for all Abstract Syntax Tree (AST) nodes.
 *
 * Every mathematical expression is represented as a tree whose nodes
 * derive from this class.
 */
class Node {
   public:
    virtual ~Node() = default;
};

using NodePtr = std::unique_ptr<Node>;

/**
 * @brief Represents a numeric literal.
 */
class NumberNode : public Node {
   public:
    explicit NumberNode(std::string value) : value(std::move(value)) {}

    /// Original textual representation of the number.
    std::string value;
};

/**
 * @brief Represents an identifier.
 *
 * Identifiers may correspond to variables or function names.
 */
class IdentifierNode : public Node {
   public:
    explicit IdentifierNode(std::string name) : name(std::move(name)) {}

    /// Identifier name.
    std::string name;
};

/**
 * @brief Unary operators.
 */
enum class UnaryOp { Plus, Minus };

/**
 * @brief Represents a unary expression.
 *
 * Examples:
 *   -x
 *   +y
 */
class UnaryNode : public Node {
   public:
    UnaryNode(UnaryOp op, NodePtr operand)
        : op(op), operand(std::move(operand)) {}

    UnaryOp op;
    NodePtr operand;
};

/**
 * @brief Binary operators.
 */
enum class BinaryOp { Add, Subtract, Multiply, Divide, Power };

/**
 * @brief Represents a binary expression.
 *
 * Examples:
 *   a + b
 *   x^2
 *   3*y
 */
class BinaryNode : public Node {
   public:
    BinaryNode(BinaryOp op, NodePtr left, NodePtr right)
        : op(op), left(std::move(left)), right(std::move(right)) {}

    BinaryOp op;
    NodePtr left;
    NodePtr right;
};

/**
 * @brief Represents a function call.
 *
 * Examples:
 *   sin(x)
 *   log(x,10)
 */
class FunctionCallNode : public Node {
   public:
    FunctionCallNode(std::string name, std::vector<NodePtr> arguments)
        : name(std::move(name)), arguments(std::move(arguments)) {}

    /// Function name.
    std::string name;

    /// Function arguments.
    std::vector<NodePtr> arguments;
};

}  // namespace numathap::ast