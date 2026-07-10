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

using MathNodePtr = std::unique_ptr<MathNode>;

/**
 * @brief Represents a numeric literal.
 *
 * The original textual representation is preserved until evaluation.
 */
class NumberNode : public MathNode {
   public:
    explicit NumberNode(std::string value)
        : value(std::move(value)) {}

    /// Original textual representation of the number.
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
    explicit SymbolNode(std::string name)
        : name(std::move(name)) {}

    /// Symbol name.
    std::string name;
};

/**
 * @brief Unary mathematical operators.
 */
enum class UnaryOp {
    Plus,
    Minus
};

/**
 * @brief Represents a unary mathematical operation.
 */
class UnaryNode : public MathNode {
   public:
    UnaryNode(UnaryOp op, MathNodePtr operand)
        : op(op),
          operand(std::move(operand)) {}

    /// Unary operator.
    UnaryOp op;

    /// Operand.
    MathNodePtr operand;
};

/**
 * @brief Binary mathematical operators.
 */
enum class BinaryOp {
    Add,
    Subtract,
    Multiply,
    Divide,
    Power
};

/**
 * @brief Represents a binary mathematical operation.
 */
class BinaryNode : public MathNode {
   public:
    BinaryNode(BinaryOp op,
               MathNodePtr left,
               MathNodePtr right)
        : op(op),
          left(std::move(left)),
          right(std::move(right)) {}

    /// Binary operator.
    BinaryOp op;

    /// Left operand.
    MathNodePtr left;

    /// Right operand.
    MathNodePtr right;
};

/**
 * @brief Represents a mathematical function.
 *
 * Examples:
 *   sin(x)
 *   cos(x)
 *   sqrt(x)
 *   abs(x)
 *   factorial(x)
 *   log(x, 10)
 *
 * Both intrinsic and user-defined functions are represented by this
 * node.
 */
class FunctionNode : public MathNode {
   public:
    FunctionNode(std::string name,
                 std::vector<MathNodePtr> arguments)
        : name(std::move(name)),
          arguments(std::move(arguments)) {}

    /// Function name.
    std::string name;

    /// Function arguments.
    std::vector<MathNodePtr> arguments;
};

}  // namespace numathap::math