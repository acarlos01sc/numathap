#include <sstream>
#include <string>

#include "numathap/math/MathAst.hpp"
#include "numathap/parser/ParserAst.hpp"

#include "PrinterTextParser.hpp"
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

std::string printParserAst(const std::string& expression) {
    numathap::parser::ParserAst ast(expression);
    std::ostringstream oss;
    ast.print(oss);
    return oss.str();
}

std::string printMathAst(const numathap::parser::ParserAst& parserAst) {
    numathap::math::MathAst mathAst(parserAst);
    std::ostringstream oss;
    mathAst.print(oss);
    return oss.str();
}

} // namespace

TEST(Integration, ParserAstRealOutput_SimpleProduct) {
    const std::string text = printParserAst("1e12*1");

    PrinterTextParser parser;
    const RenderNode root = parser.parse(text);

    EXPECT_EQ(root.label, "Binary(*)");
    EXPECT_EQ(root.children.size(), 2u);
    EXPECT_EQ(root.children[0].label, "Number(1e12)");
    EXPECT_EQ(root.children[1].label, "Number(1)");
}

TEST(Integration, MathAstRealOutput_SimpleProduct) {
    numathap::parser::ParserAst parserAst("1e12*1");
    const std::string text = printMathAst(parserAst);

    PrinterTextParser parser;
    const RenderNode root = parser.parse(text);

    EXPECT_EQ(root.label, "Binary(*)");
    EXPECT_EQ(root.children.size(), 2u);
    EXPECT_EQ(root.children[0].label, "Number(1e12)");
    EXPECT_EQ(root.children[1].label, "Number(1)");
}

TEST(Integration, ParserAstRealOutput_NestedBinary) {
    const std::string text = printParserAst("1+2*3");

    PrinterTextParser parser;
    const RenderNode root = parser.parse(text);

    EXPECT_EQ(root.label, "Binary(+)");
    EXPECT_EQ(root.children.size(), 2u);
    // Binary(+), Number(1), Binary(*), Number(2), Number(3)
    EXPECT_EQ(countNodes(root), 5u);
}

TEST(Integration, MathAstRealOutput_NestedBinary) {
    numathap::parser::ParserAst parserAst("1+2*3");
    const std::string text = printMathAst(parserAst);

    PrinterTextParser parser;
    const RenderNode root = parser.parse(text);

    EXPECT_EQ(root.label, "Binary(+)");
    EXPECT_EQ(root.children.size(), 2u);
    EXPECT_EQ(countNodes(root), 5u);
}

int main(int argc, char** argv) {
    const std::string filter = (argc > 1) ? argv[1] : "";
    return testfw::runFiltered(filter);
}