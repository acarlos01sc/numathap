#include "numathap/parser/ParserAstPrinter.hpp"

namespace numathap::parser {


void ParserAstPrinter::print(
    const Node* node,
    std::ostream& os
) const
{
    printNode(node, os, 0);
}


void ParserAstPrinter::printNode(
    const Node* node,
    std::ostream& os,
    int depth
) const
{
    if (!node)
        return;


    if (auto n = dynamic_cast<const NumberNode*>(node))
    {
        indent(os, depth);
        os << "Number(" << n->value << ")\n";
    }
    else if (auto n = dynamic_cast<const IdentifierNode*>(node))
    {
        indent(os, depth);
        os << "Identifier(" << n->name << ")\n";
    }
    else if (auto n = dynamic_cast<const UnaryNode*>(node))
    {
        printUnary(n, os, depth);
    }
    else if (auto n = dynamic_cast<const BinaryNode*>(node))
    {
        printBinary(n, os, depth);
    }
    else if (auto n = dynamic_cast<const PostfixNode*>(node))
    {
        printPostfix(n, os, depth);
    }
    else if (auto n = dynamic_cast<const FunctionCallNode*>(node))
    {
        printFunction(n, os, depth);
    }
    else if (auto n = dynamic_cast<const AbsoluteNode*>(node))
    {
        printAbsolute(n, os, depth);
    }
}


void ParserAstPrinter::indent(
    std::ostream& os,
    int depth
) const
{
    for (int i = 0; i < depth; ++i)
        os << "  ";
}


void ParserAstPrinter::printUnary(
    const UnaryNode* node,
    std::ostream& os,
    int depth
) const
{
    indent(os, depth);

    os << "Unary("
       << unaryName(node->op)
       << ")\n";

    printNode(node->operand.get(), os, depth + 1);
}


void ParserAstPrinter::printBinary(
    const BinaryNode* node,
    std::ostream& os,
    int depth
) const
{
    indent(os, depth);

    os << "Binary("
       << binaryName(node->op)
       << ")\n";


    printNode(node->left.get(), os, depth + 1);
    printNode(node->right.get(), os, depth + 1);
}


void ParserAstPrinter::printPostfix(
    const PostfixNode* node,
    std::ostream& os,
    int depth
) const
{
    indent(os, depth);

    os << "Postfix("
       << postfixName(node->op)
       << ")\n";


    printNode(node->operand.get(), os, depth + 1);
}


void ParserAstPrinter::printFunction(
    const FunctionCallNode* node,
    std::ostream& os,
    int depth
) const
{
    indent(os, depth);

    os << "Function("
       << node->name
       << ")\n";


    for (const auto& arg : node->arguments)
    {
        printNode(arg.get(), os, depth + 1);
    }
}


void ParserAstPrinter::printAbsolute(
    const AbsoluteNode* node,
    std::ostream& os,
    int depth
) const
{
    indent(os, depth);

    os << "Absolute\n";

    printNode(node->operand.get(), os, depth + 1);
}


const char* ParserAstPrinter::unaryName(UnaryOp op) const
{
    switch(op)
    {
        case UnaryOp::Plus:
            return "+";

        case UnaryOp::Minus:
            return "-";
    }

    return "?";
}


const char* ParserAstPrinter::binaryName(BinaryOp op) const
{
    switch(op)
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


const char* ParserAstPrinter::postfixName(PostfixOp op) const
{
    switch(op)
    {
        case PostfixOp::Factorial:
            return "!";
    }

    return "?";
}


} // namespace numathap::parser