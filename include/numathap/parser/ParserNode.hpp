#pragma once

#include <memory>
#include <string>
#include <vector>

namespace numathap::parser {

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
 *
 * The original textual representation is preserved exactly as it
 * appears in the input.
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
 * Identifiers may represent any symbolic name.
 *
 * Their semantic interpretation is performed in later stages.
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
 * A unary expression consists of a unary operator applied to a single
 * operand.
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
 * A binary expression consists of a binary operator applied to two
 * operands.
 *
 * Examples:
 *   a + b
 *   x^2
 *   3 * y
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
 * @brief Postfix operators.
 */
enum class PostfixOp { Factorial };

/**
 * @brief Represents a postfix unary expression.
 *
 * A postfix unary expression consists of a postfix operator applied to
 * a single operand.
 *
 * Examples:
 *   x!
 */
class PostfixNode : public Node {
   public:
    PostfixNode(PostfixOp op, NodePtr operand)
        : op(op), operand(std::move(operand)) {}

    PostfixOp op;
    NodePtr operand;
};

/**
 * @brief Represents a function call.
 *
 * A function call consists of a function name followed by zero or more
 * arguments.
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

/**
 * @brief Represents an absolute value expression.
 *
 * An absolute value expression consists of an expression enclosed
 * by vertical bars.
 *
 * Examples:
 *   |x|
 *   |a-b|
 */
class AbsoluteNode : public Node {
   public:
    explicit AbsoluteNode(NodePtr operand) : operand(std::move(operand)) {}

    NodePtr operand;
};

}  // namespace numathap::parser