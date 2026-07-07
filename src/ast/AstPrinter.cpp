#include "numathap/ast/AstPrinter.hpp"

#include <iostream>


namespace numathap::ast {

namespace {

std::string binaryOpName(BinaryOp op)
{
    switch (op) {
        case BinaryOp::Add:
            return "Add";
        case BinaryOp::Subtract:
            return "Subtract";
        case BinaryOp::Multiply:
            return "Multiply";
        case BinaryOp::Divide:
            return "Divide";
        case BinaryOp::Power:
            return "Power";
    }

    return "Unknown";
}

std::string unaryOpName(UnaryOp op)
{
    switch (op) {
        case UnaryOp::Plus:
            return "Plus";
        case UnaryOp::Minus:
            return "Minus";
    }

    return "Unknown";
}

std::string postfixOpName(PostfixOp op)
{
    switch (op) {
        case PostfixOp::Factorial:
            return "Factorial";
    }

    return "Unknown";
}

}  // namespace


void AstPrinter::print(const Node& node, std::ostream& out)
{
    printNode(node, out, "", true);
}


void AstPrinter::printNode(const Node& node,
                           std::ostream& out,
                           const std::string& prefix,
                           bool isLast)
{
    out << prefix;

    if (!prefix.empty()) {
        out << (isLast ? "└── " : "├── ");
    }


    if (auto number = dynamic_cast<const NumberNode*>(&node)) {

        out << "Number(" << number->value << ")\n";

    } else if (auto identifier =
                   dynamic_cast<const IdentifierNode*>(&node)) {

        out << "Identifier(" << identifier->name << ")\n";

    } else if (auto unary =
                   dynamic_cast<const UnaryNode*>(&node)) {

        out << "Unary(" << unaryOpName(unary->op) << ")\n";

        printNode(*unary->operand,
                  out,
                  prefix + (isLast ? "    " : "│   "),
                  true);

    } else if (auto binary =
                   dynamic_cast<const BinaryNode*>(&node)) {

        out << "Binary(" << binaryOpName(binary->op) << ")\n";

        printNode(*binary->left,
                  out,
                  prefix + (isLast ? "    " : "│   "),
                  false);

        printNode(*binary->right,
                  out,
                  prefix + (isLast ? "    " : "│   "),
                  true);

    } else if (auto postfix =
                   dynamic_cast<const PostfixNode*>(&node)) {

        out << "Postfix(" << postfixOpName(postfix->op) << ")\n";

        printNode(*postfix->operand,
                  out,
                  prefix + (isLast ? "    " : "│   "),
                  true);

    } else if (auto function =
                   dynamic_cast<const FunctionCallNode*>(&node)) {

        out << "FunctionCall(" << function->name << ")\n";

        for (std::size_t i = 0; i < function->arguments.size(); ++i) {

            printNode(*function->arguments[i],
                      out,
                      prefix + (isLast ? "    " : "│   "),
                      i == function->arguments.size() - 1);
        }

    } else if (auto absolute =
                   dynamic_cast<const AbsoluteNode*>(&node)) {

        out << "Absolute\n";

        printNode(*absolute->operand,
                  out,
                  prefix + (isLast ? "    " : "│   "),
                  true);

    } else {
        out << "UnknownNode\n";
    }
}

}  // namespace numathap::ast