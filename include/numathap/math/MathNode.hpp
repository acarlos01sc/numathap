/**
 * @file MathNode.hpp
 * @brief Base definitions for the Math-AST node hierarchy.
 *
 * This file defines the core structures of the semantic Abstract Syntax Tree
 * used by the numathap library to represent mathematical expressions.
 */
#pragma once

#include <memory>
#include <string>
#include <vector>

namespace numathap::math {

/**
 * @brief Base class for all mathematical Abstract Syntax Tree (Math-AST)
 * nodes.
 *
 * Unlike the parser AST, the Math-AST represents the semantic meaning
 * of an expression rather than its syntactic form.
 */
class MathNode {
   public:
    virtual ~MathNode() = default;
};

/**
 * @brief Alias for a unique pointer to a MathNode.
 */
using MathNodePtr = std::unique_ptr<MathNode>;

/**
 * @brief Represents a numeric literal.
 *
 * The original textual representation is preserved until evaluation.
 */
class NumberNode : public MathNode {
   public:
    /**
     * @brief Constructs a new NumberNode.
     * @param value The original textual representation of the number.
     */
    explicit NumberNode(std::string value) : value(std::move(value)) {}

    /// @brief Original textual representation of the number.
    std::string value;
};

/**
 * @brief Represents a mathematical symbol.
 *
 * A symbol may later be resolved as a variable, a named constant,
 * or another semantic entity by subsequent processing stages.
 */
class SymbolNode : public MathNode {
   public:
    /**
     * @brief Constructs a new SymbolNode.
     * @param name The name of the symbol.
     */
    explicit SymbolNode(std::string name) : name(std::move(name)) {}

    /// @brief Symbol name.
    std::string name;
};

/**
 * @brief Unary mathematical operators.
 */
enum class UnaryOp { Plus, Minus };

/**
 * @brief Represents a unary mathematical operation.
 */
class UnaryNode : public MathNode {
   public:
    /**
     * @brief Constructs a new UnaryNode.
     * @param op The unary operator.
     * @param operand The node to which the operator is applied.
     */
    UnaryNode(UnaryOp op, MathNodePtr operand)
        : op(op), operand(std::move(operand)) {}

    /// @brief Unary operator.
    UnaryOp op;

    /// @brief Operand node.
    MathNodePtr operand;
};

/**
 * @brief Binary mathematical operators.
 */
enum class BinaryOp {
    Add,       ///< Addition operator (+)
    Subtract,  ///< Subtraction operator (-)
    Multiply,  ///< Multiplication operator (*)
    Divide,    ///< Division operator (/)
    Power      ///< Exponentiation operator (^)
};

/**
 * @brief Represents a binary mathematical operation.
 */
class BinaryNode : public MathNode {
   public:
    /**
     * @brief Constructs a new BinaryNode.
     * @param op The binary operator.
     * @param left The left operand node.
     * @param right The right operand node.
     */
    BinaryNode(BinaryOp op, MathNodePtr left, MathNodePtr right)
        : op(op), left(std::move(left)), right(std::move(right)) {}

    /// @brief Binary operator.
    BinaryOp op;

    /// @brief Left operand.
    MathNodePtr left;

    /// @brief Right operand.
    MathNodePtr right;
};

/**
 * @brief Represents a mathematical function.
 *
 * Examples:
 *   sin(x), cos(x), sqrt(x), abs(x), factorial(x), log(x, 10)
 *
 * Both intrinsic and user-defined functions are represented by this
 * node.
 */
class FunctionNode : public MathNode {
   public:
    /**
     * @brief Constructs a new FunctionNode.
     * @param name The name of the function.
     * @param arguments A vector containing the function's arguments.
     */
    FunctionNode(std::string name, std::vector<MathNodePtr> arguments)
        : name(std::move(name)), arguments(std::move(arguments)) {}

    /// @brief Function name.
    std::string name;

    /// @brief List of function arguments.
    std::vector<MathNodePtr> arguments;
};

}  // namespace numathap::math