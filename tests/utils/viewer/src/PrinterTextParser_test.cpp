#include "PrinterTextParser.hpp"

#include <stdexcept>
#include <string>

#include "test_framework.hpp"

using numathap::viewer::PrinterTextParser;
using numathap::viewer::RenderNode;

namespace {

std::size_t countNodes(const RenderNode& node) {
    std::size_t total = 1;
    for (const auto& child : node.children) {
        total += countNodes(child);
    }
    return total;
}

}  // namespace

TEST(PrinterTextParser, BinaryExpression) {
    const std::string text =
        "Binary(*)\n"
        "  Number(1e12)\n"
        "  Number(1)\n";

    PrinterTextParser parser;
    const RenderNode root = parser.parse(text);

    EXPECT_EQ(root.label, "Binary(*)");
    EXPECT_EQ(root.children.size(), 2u);
    EXPECT_EQ(root.children[0].label, "Number(1e12)");
    EXPECT_EQ(root.children[1].label, "Number(1)");
}

TEST(PrinterTextParser, FunctionWithMultipleArguments) {
    const std::string text =
        "Function(log)\n"
        "  Identifier(x)\n"
        "  Number(10)\n";

    PrinterTextParser parser;
    const RenderNode root = parser.parse(text);

    EXPECT_EQ(root.label, "Function(log)");
    EXPECT_EQ(root.children.size(), 2u);
}

TEST(PrinterTextParser, MultiLevelDedent) {
    const std::string text =
        "Binary(+)\n"
        "  Unary(-)\n"
        "    Postfix(!)\n"
        "      Identifier(x)\n"
        "  Number(1)\n";

    PrinterTextParser parser;
    const RenderNode root = parser.parse(text);

    EXPECT_EQ(root.children.size(), 2u);
    EXPECT_EQ(root.children[0].label, "Unary(-)");
    EXPECT_EQ(root.children[0].children.at(0).label, "Postfix(!)");
    EXPECT_EQ(root.children[1].label, "Number(1)");
    EXPECT_EQ(countNodes(root), 5u);
}

TEST(PrinterTextParser, MalformedIndentationIsRejected) {
    const std::string text =
        "Binary(*)\n"
        "  Number(1)\n"
        "   Number(2)\n";  // 3 espaços: não alinhado à unidade detectada (2)

    PrinterTextParser parser;
    EXPECT_THROWS(parser.parse(text), std::invalid_argument);
}

TEST(PrinterTextParser, ConnectorStyleBinaryExpression) {
    const std::string text =
        "Binary(*)\n"
        "\u2514\u2500\u2500 Number(1e12)\n";  // caso degenerado: só 1 filho,
                                              // para focar no parsing do
                                              // conector

    PrinterTextParser parser;
    const RenderNode root = parser.parse(text);

    EXPECT_EQ(root.label, "Binary(*)");
    EXPECT_EQ(root.children.size(), 1u);
    EXPECT_EQ(root.children[0].label, "Number(1e12)");
}

TEST(PrinterTextParser, ConnectorStyleDeepNesting) {
    // Mirrors MathAstPrinter's real output for:
    //   Binary(+) { left: Unary(-) { Function(log){Symbol(x), Number(10)} },
    //               right: Number(1) }
    const std::string text =
        "Binary(+)\n"
        "    \u251C\u2500\u2500 Unary(-)\n"
        "    \u2502   \u2514\u2500\u2500 Function(log)\n"
        "    \u2502       \u251C\u2500\u2500 Symbol(x)\n"
        "    \u2502       \u2514\u2500\u2500 Number(10)\n"
        "    \u2514\u2500\u2500 Number(1)\n";

    PrinterTextParser parser;
    const RenderNode root = parser.parse(text);

    EXPECT_EQ(root.children.size(), 2u);
    EXPECT_EQ(root.children[0].label, "Unary(-)");
    EXPECT_EQ(root.children[0].children.size(), 1u);
    EXPECT_EQ(root.children[0].children[0].label, "Function(log)");
    EXPECT_EQ(root.children[0].children[0].children.size(), 2u);
    EXPECT_EQ(root.children[0].children[0].children[0].label, "Symbol(x)");
    EXPECT_EQ(root.children[0].children[0].children[1].label, "Number(10)");
    EXPECT_EQ(root.children[1].label, "Number(1)");
}

TEST(PrinterTextParser, ConnectorStyleMalformedPrefixIsRejected) {
    const std::string text =
        "Binary(*)\n"
        "\u251C Number(1)\n";  // "├" sem o "── " completo

    PrinterTextParser parser;
    EXPECT_THROWS(parser.parse(text), std::invalid_argument);
}

int main(int argc, char** argv) {
    const std::string filter = (argc > 1) ? argv[1] : "";
    return testfw::runFiltered(filter);
}