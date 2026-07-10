#include "numathap/math/MathAstBuilder.hpp"

#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "numathap/math/MathNode.hpp"
#include "numathap/parser/ast/Node.hpp"

namespace numathap::math {

MathNodePtr MathAstBuilder::build(const ast::NodePtr& root) const {
    if (!root) {
        return nullptr;
    }

    return buildNode(*root);
}

MathNodePtr MathAstBuilder::buildNode(const ast::Node& node) const {
    //----------------------------------------------------------
    // Number
    //----------------------------------------------------------

    if (const auto* n = dynamic_cast<const ast::NumberNode*>(&node)) {
        return std::make_unique<NumberNode>(n->value);
    }

    //----------------------------------------------------------
    // Identifier -> Symbol
    //----------------------------------------------------------

    if (const auto* n = dynamic_cast<const ast::IdentifierNode*>(&node)) {
        return std::make_unique<SymbolNode>(n->name);
    }

    //----------------------------------------------------------
    // Unary
    //----------------------------------------------------------

    if (const auto* n = dynamic_cast<const ast::UnaryNode*>(&node)) {
        UnaryOp op;

        switch (n->op) {
            case ast::UnaryOp::Plus:
                op = UnaryOp::Plus;
                break;

            case ast::UnaryOp::Minus:
                op = UnaryOp::Minus;
                break;
        }

        return std::make_unique<UnaryNode>(
            op,
            buildNode(*n->operand));
    }

    //----------------------------------------------------------
    // Binary
    //----------------------------------------------------------

    if (const auto* n = dynamic_cast<const ast::BinaryNode*>(&node)) {
        BinaryOp op;

        switch (n->op) {
            case ast::BinaryOp::Add:
                op = BinaryOp::Add;
                break;

            case ast::BinaryOp::Subtract:
                op = BinaryOp::Subtract;
                break;

            case ast::BinaryOp::Multiply:
                op = BinaryOp::Multiply;
                break;

            case ast::BinaryOp::Divide:
                op = BinaryOp::Divide;
                break;

            case ast::BinaryOp::Power:
                op = BinaryOp::Power;
                break;
        }

        return std::make_unique<BinaryNode>(
            op,
            buildNode(*n->left),
            buildNode(*n->right));
    }

    //----------------------------------------------------------
    // Function call
    //----------------------------------------------------------

    if (const auto* n = dynamic_cast<const ast::FunctionCallNode*>(&node)) {
        std::vector<MathNodePtr> arguments;
        arguments.reserve(n->arguments.size());

        for (const auto& argument : n->arguments) {
            arguments.push_back(buildNode(*argument));
        }

        return std::make_unique<FunctionNode>(
            n->name,
            std::move(arguments));
    }

    //----------------------------------------------------------
    // Absolute value
    //----------------------------------------------------------

    if (const auto* n = dynamic_cast<const ast::AbsoluteNode*>(&node)) {
        std::vector<MathNodePtr> arguments;
        arguments.push_back(buildNode(*n->operand));

        return std::make_unique<FunctionNode>(
            "abs",
            std::move(arguments));
    }

    //----------------------------------------------------------
    // Postfix operators
    //----------------------------------------------------------

    if (const auto* n = dynamic_cast<const ast::PostfixNode*>(&node)) {
        switch (n->op) {
            case ast::PostfixOp::Factorial: {
                std::vector<MathNodePtr> arguments;
                arguments.push_back(buildNode(*n->operand));

                return std::make_unique<FunctionNode>(
                    "factorial",
                    std::move(arguments));
            }
        }
    }

    throw std::logic_error("Unsupported parser AST node.");
}

}  // namespace numathap::math