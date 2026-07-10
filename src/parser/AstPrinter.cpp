#include "numathap/parser/AstPrinter.hpp"

#include <sstream>
#include <stdexcept>

namespace numathap::parser {

using namespace ast;

std::string AstPrinter::toString(const Node& node) const
{
    std::ostringstream out;
    print(out, node);
    return out.str();
}

std::string AstPrinter::toString(const NodePtr& node) const
{
    if (!node)
        return {};

    return toString(*node);
}

void AstPrinter::print(std::ostream& os, const Node& node) const
{
    printNode(os, node, "", true);
}

void AstPrinter::print(std::ostream& os, const NodePtr& node) const
{
    if (!node)
        return;

    print(os, *node);
}

void AstPrinter::printNode(std::ostream& os,
                           const Node& node,
                           const std::string& prefix,
                           bool isLast) const
{
    os << prefix
       << (isLast ? "`-- " : "|-- ");

    const std::string childPrefix =
        prefix + (isLast ? "    " : "|   ");

    //------------------------------------------------------------------
    // Number
    //------------------------------------------------------------------

    if (auto p = dynamic_cast<const NumberNode*>(&node))
    {
        os << "Number(" << p->value << ")\n";
        return;
    }

    //------------------------------------------------------------------
    // Identifier
    //------------------------------------------------------------------

    if (auto p = dynamic_cast<const IdentifierNode*>(&node))
    {
        os << "Identifier(" << p->name << ")\n";
        return;
    }

    //------------------------------------------------------------------
    // Unary
    //------------------------------------------------------------------

    if (auto p = dynamic_cast<const UnaryNode*>(&node))
    {
        os << "Unary(" << unaryOpName(p->op) << ")\n";

        printNode(os,
                  *p->operand,
                  childPrefix,
                  true);

        return;
    }

    //------------------------------------------------------------------
    // Binary
    //------------------------------------------------------------------

    if (auto p = dynamic_cast<const BinaryNode*>(&node))
    {
        os << "Binary(" << binaryOpName(p->op) << ")\n";

        printNode(os,
                  *p->left,
                  childPrefix,
                  false);

        printNode(os,
                  *p->right,
                  childPrefix,
                  true);

        return;
    }

    //------------------------------------------------------------------
    // Postfix
    //------------------------------------------------------------------

    if (auto p = dynamic_cast<const PostfixNode*>(&node))
    {
        os << "Postfix(" << postfixOpName(p->op) << ")\n";

        printNode(os,
                  *p->operand,
                  childPrefix,
                  true);

        return;
    }

    //------------------------------------------------------------------
    // Function
    //------------------------------------------------------------------

    if (auto p = dynamic_cast<const FunctionCallNode*>(&node))
    {
        os << "Function(" << p->name << ")\n";

        for (std::size_t i = 0; i < p->arguments.size(); ++i)
        {
            printNode(os,
                      *p->arguments[i],
                      childPrefix,
                      i + 1 == p->arguments.size());
        }

        return;
    }

    //------------------------------------------------------------------
    // Absolute
    //------------------------------------------------------------------

    if (auto p = dynamic_cast<const AbsoluteNode*>(&node))
    {
        os << "Absolute\n";

        printNode(os,
                  *p->operand,
                  childPrefix,
                  true);

        return;
    }

    throw std::runtime_error("Unknown AST node.");
}

std::string AstPrinter::unaryOpName(UnaryOp op)
{
    switch (op)
    {
        case UnaryOp::Plus:
            return "+";

        case UnaryOp::Minus:
            return "-";
    }

    return "?";
}

std::string AstPrinter::binaryOpName(BinaryOp op)
{
    switch (op)
    {
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

std::string AstPrinter::postfixOpName(PostfixOp op)
{
    switch (op)
    {
        case PostfixOp::Factorial:
            return "!";
    }

    return "?";
}

} // namespace numathap::parser