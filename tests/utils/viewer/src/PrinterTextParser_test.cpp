#include <iostream>
#include <stdexcept>
#include <string>

#include "PrinterTextParser.hpp"

namespace {

using numathap::viewer::PrinterTextParser;
using numathap::viewer::RenderNode;

int g_failures = 0;

void printRenderNode(const RenderNode& node, std::ostream& os, int depth = 0)
{
    for (int i = 0; i < depth; ++i)
    {
        os << "  ";
    }
    os << node.label << '\n';
    for (const auto& child : node.children)
    {
        printRenderNode(child, os, depth + 1);
    }
}

std::size_t countNodes(const RenderNode& node)
{
    std::size_t total = 1;
    for (const auto& child : node.children)
    {
        total += countNodes(child);
    }
    return total;
}

void check(bool condition, const std::string& description)
{
    if (condition)
    {
        std::cout << "  [OK] " << description << '\n';
    }
    else
    {
        std::cout << "  [FAIL] " << description << '\n';
        ++g_failures;
    }
}

} // namespace

int main()
{
    // Case 1: binary expression - mirrors ParserAstPrinter output for "1e12*1"
    {
        const std::string text =
            "Binary(*)\n"
            "  Number(1e12)\n"
            "  Number(1)\n";

        std::cout << "\n============================================================\n";
        std::cout << "Case: binary expression\n";
        std::cout << "------------------------------------------------------------\n";
        std::cout << "Input text:\n" << text;

        PrinterTextParser parser;
        const RenderNode root = parser.parse(text);

        std::cout << "Reconstructed tree:\n";
        printRenderNode(root, std::cout);

        check(root.label == "Binary(*)", "root label is Binary(*)");
        check(root.children.size() == 2, "root has 2 children");
        check(root.children[0].label == "Number(1e12)", "first child is Number(1e12)");
        check(root.children[1].label == "Number(1)", "second child is Number(1)");
    }

    // Case 2: function call with multiple arguments (variable arity)
    {
        const std::string text =
            "Function(log)\n"
            "  Identifier(x)\n"
            "  Number(10)\n";

        std::cout << "\n============================================================\n";
        std::cout << "Case: function with multiple arguments\n";
        std::cout << "------------------------------------------------------------\n";
        std::cout << "Input text:\n" << text;

        PrinterTextParser parser;
        const RenderNode root = parser.parse(text);

        std::cout << "Reconstructed tree:\n";
        printRenderNode(root, std::cout);

        check(root.label == "Function(log)", "root label is Function(log)");
        check(root.children.size() == 2, "root has 2 arguments");
    }

    // Case 3: multi-level dedent (returning from a deep chain back to a
    // sibling of an ancestor)
    {
        const std::string text =
            "Binary(+)\n"
            "  Unary(-)\n"
            "    Postfix(!)\n"
            "      Identifier(x)\n"
            "  Number(1)\n";

        std::cout << "\n============================================================\n";
        std::cout << "Case: multi-level dedent\n";
        std::cout << "------------------------------------------------------------\n";
        std::cout << "Input text:\n" << text;

        PrinterTextParser parser;
        const RenderNode root = parser.parse(text);

        std::cout << "Reconstructed tree:\n";
        printRenderNode(root, std::cout);

        check(root.children.size() == 2, "root has 2 children after dedent");
        check(root.children[0].label == "Unary(-)", "first child is Unary(-)");
        check(root.children[0].children.at(0).label == "Postfix(!)",
              "Unary's child is Postfix(!)");
        check(root.children[1].label == "Number(1)",
              "second child correctly returned to depth 1");
        check(countNodes(root) == 5, "tree has 5 nodes total");
    }

    // Case 4: malformed indentation must be rejected, not silently misread
    {
        const std::string text =
            "Binary(*)\n"
            "  Number(1)\n"
            "   Number(2)\n"; // 3 spaces: not aligned to the detected unit (2)

        std::cout << "\n============================================================\n";
        std::cout << "Case: malformed indentation (expected to throw)\n";
        std::cout << "------------------------------------------------------------\n";
        std::cout << "Input text:\n" << text;

        PrinterTextParser parser;
        try
        {
            [[maybe_unused]] const RenderNode discarded = parser.parse(text);
            check(false, "malformed indentation should have thrown");
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << "Caught expected error: " << e.what() << '\n';
            check(true, "malformed indentation correctly rejected");
        }
    }

    std::cout << "\n============================================================\n";
    if (g_failures == 0)
    {
        std::cout << "All PrinterTextParser checks passed.\n";
    }
    else
    {
        std::cout << g_failures << " check(s) FAILED.\n";
    }

    return g_failures == 0 ? 0 : 1;
}