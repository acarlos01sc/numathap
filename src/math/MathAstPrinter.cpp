#include "numathap/math/MathAstPrinter.hpp"

#include <stdexcept>

namespace {

const char* toString(numathap::math::UnaryOp op) {
    using numathap::math::UnaryOp;

    switch (op) {
        case UnaryOp::Plus:
            return "+";

        case UnaryOp::Minus:
            return "-";
    }

    return "?";
}

const char* toString(numathap::math::BinaryOp op) {
    using numathap::math::BinaryOp;

    switch (op) {
        case BinaryOp::Add:
            return "+";

        case BinaryOp::Subtract:
            return "-";

        case BinaryOp::Multiply:
            return "*";

        case BinaryOp::Divide:
            return "/";

        case BinaryOp::Power:
            return "^";
    }

    return "?";
}

}  // namespace

namespace numathap::math {

void MathAstPrinter::print(const MathNode& node, std::ostream& os) const {
    printNode(os, node, "", true);
}

void MathAstPrinter::printNode(std::ostream& os, const MathNode& node,
                               const std::string& prefix, bool isLast) const {
    os << prefix;

    if (!prefix.empty()) {
        os << (isLast ? "└── " : "├── ");
    }

    //----------------------------------------------------------
    // Number
    //----------------------------------------------------------

    if (const auto* n = dynamic_cast<const NumberNode*>(&node)) {
        os << "Number(" << n->value << ")\n";
        return;
    }

    //----------------------------------------------------------
    // Symbol
    //----------------------------------------------------------

    if (const auto* n = dynamic_cast<const SymbolNode*>(&node)) {
        os << "Symbol(" << n->name << ")\n";
        return;
    }

    //----------------------------------------------------------
    // Unary
    //----------------------------------------------------------

    if (const auto* n = dynamic_cast<const UnaryNode*>(&node)) {
        os << "Unary(" << toString(n->op) << ")\n";

        printNode(os, *n->operand, prefix + (isLast ? "    " : "│   "), true);

        return;
    }

    //----------------------------------------------------------
    // Binary
    //----------------------------------------------------------

    if (const auto* n = dynamic_cast<const BinaryNode*>(&node)) {
        os << "Binary(" << toString(n->op) << ")\n";

        const std::string childPrefix = prefix + (isLast ? "    " : "│   ");

        printNode(os, *n->left, childPrefix, false);

        printNode(os, *n->right, childPrefix, true);

        return;
    }

    //----------------------------------------------------------
    // Function
    //----------------------------------------------------------

    if (const auto* n = dynamic_cast<const FunctionNode*>(&node)) {
        os << "Function(" << n->name << ")\n";

        const std::string childPrefix = prefix + (isLast ? "    " : "│   ");

        for (std::size_t i = 0; i < n->arguments.size(); ++i) {
            printNode(os, *n->arguments[i], childPrefix,
                      i + 1 == n->arguments.size());
        }

        return;
    }

    //----------------------------------------------------------

    throw std::logic_error("Unknown MathNode.");
}

}  // namespace numathap::math