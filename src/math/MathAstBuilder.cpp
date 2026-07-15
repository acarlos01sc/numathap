#include "numathap/math/MathAstBuilder.hpp"

#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "numathap/math/MathNode.hpp"
#include "numathap/parser/ParserNode.hpp"

namespace numathap::math {

MathNodePtr MathAstBuilder::build(const parser::Node* root) const {
    if (!root) {
        return nullptr;
    }

    return buildNode(*root);
}

MathNodePtr MathAstBuilder::buildNode(const parser::Node& node) const {
    //----------------------------------------------------------
    // Number
    //----------------------------------------------------------

    if (const auto* n = dynamic_cast<const parser::NumberNode*>(&node)) {
        return std::make_unique<NumberNode>(n->value);
    }

    //----------------------------------------------------------
    // Identifier -> Symbol
    //----------------------------------------------------------

    if (const auto* n = dynamic_cast<const parser::IdentifierNode*>(&node)) {
        return std::make_unique<SymbolNode>(n->name);
    }

    //----------------------------------------------------------
    // Unary
    //----------------------------------------------------------

    if (const auto* n = dynamic_cast<const parser::UnaryNode*>(&node)) {
        UnaryOp op;

        switch (n->op) {
            case parser::UnaryOp::Plus:
                op = UnaryOp::Plus;
                break;

            case parser::UnaryOp::Minus:
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

    if (const auto* n = dynamic_cast<const parser::BinaryNode*>(&node)) {
        BinaryOp op;

        switch (n->op) {
            case parser::BinaryOp::Add:
                op = BinaryOp::Add;
                break;

            case parser::BinaryOp::Subtract:
                op = BinaryOp::Subtract;
                break;

            case parser::BinaryOp::Multiply:
                op = BinaryOp::Multiply;
                break;

            case parser::BinaryOp::Divide:
                op = BinaryOp::Divide;
                break;

            case parser::BinaryOp::Power:
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

    if (const auto* n = dynamic_cast<const parser::FunctionCallNode*>(&node)) {
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

    if (const auto* n = dynamic_cast<const parser::AbsoluteNode*>(&node)) {
        std::vector<MathNodePtr> arguments;
        arguments.push_back(buildNode(*n->operand));

        return std::make_unique<FunctionNode>(
            "abs",
            std::move(arguments));
    }

    //----------------------------------------------------------
    // Postfix operators
    //----------------------------------------------------------

    if (const auto* n = dynamic_cast<const parser::PostfixNode*>(&node)) {
        switch (n->op) {
            case parser::PostfixOp::Factorial: {
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