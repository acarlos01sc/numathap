#include "PrinterTextParser.hpp"
#include "test_framework.hpp"

#include <stdexcept>
#include <string>

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

} // namespace

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

TEST(PrinterTextParser, FunctionMultipleArguments) {
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
        "   Number(2)\n"; // 3 espaços: não alinha à unidade detectada (2)

    PrinterTextParser parser;
    EXPECT_THROWS(parser.parse(text), std::invalid_argument);
}

// --- Casos novos ---

TEST(PrinterTextParser, SingleNodeTree) {
    // Sem descida, não há delta positivo para detectar a unidade de
    // indentação -- exercita o fallback em detectIndentUnit().
    const std::string text = "Number(42)\n";

    PrinterTextParser parser;
    const RenderNode root = parser.parse(text);

    EXPECT_EQ(root.label, "Number(42)");
    EXPECT_TRUE(root.children.empty());
}

TEST(PrinterTextParser, EmptyTextIsRejected) {
    PrinterTextParser parser;
    EXPECT_THROWS(parser.parse(""), std::invalid_argument);
}

TEST(PrinterTextParser, BlankTextIsRejected) {
    PrinterTextParser parser;
    EXPECT_THROWS(parser.parse("   \n\t\n  \n"), std::invalid_argument);
}

TEST(PrinterTextParser, MultipleRootNodesAreRejected) {
    // Duas árvores de profundidade 0 concatenadas -- deve ser rejeitado
    // como "mais de um nó raiz", conforme documentado em parse().
    const std::string text =
        "Binary(*)\n"
        "  Number(1)\n"
        "Binary(+)\n"
        "  Number(2)\n";

    PrinterTextParser parser;
    EXPECT_THROWS(parser.parse(text), std::invalid_argument);
}

int main(int argc, char** argv) {
    const std::string filter = (argc > 1) ? argv[1] : "";
    return testfw::runFiltered(filter);
}