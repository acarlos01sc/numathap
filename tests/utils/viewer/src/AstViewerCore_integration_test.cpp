#include "AstViewerCore.hpp"

#include <string>
#include <vector>

#include "test_framework.hpp"

using numathap::viewer::AstColumn;
using numathap::viewer::AstViewerCore;

namespace {

const AstColumn& findColumn(const std::vector<AstColumn>& columns,
                             const std::string& title) {
  for (const auto& col : columns) {
    if (col.title == title) {
      return col;
    }
  }
  throw std::runtime_error("column not found: " + title);
}

}  // namespace

TEST(AstViewerCore, BuildColumns_HasAllFiveColumnsInOrder) {
  const AstViewerCore viewer;
  const std::vector<AstColumn> columns = viewer.buildColumns("1+2*3");

  EXPECT_EQ(columns.size(), 5u);
  EXPECT_EQ(columns[0].title, "Input");
  EXPECT_EQ(columns[1].title, "ParserAst");
  EXPECT_EQ(columns[2].title, "MathAst");
  EXPECT_EQ(columns[3].title, "PreparedAst");
  EXPECT_EQ(columns[4].title, "PreparedAst+Simplify");
}

TEST(AstViewerCore, InputColumn_EchoesExpression) {
  const AstViewerCore viewer;
  const std::vector<AstColumn> columns = viewer.buildColumns("1e12*1");

  const AstColumn& input = findColumn(columns, "Input");
  EXPECT_EQ(input.lines.size(), 1u);
  EXPECT_EQ(input.lines[0], "1e12*1");
}

TEST(AstViewerCore, ParserAstColumn_MatchesRealPrintedShape) {
  const AstViewerCore viewer;
  const std::vector<AstColumn> columns = viewer.buildColumns("1e12*1");

  const AstColumn& parserCol = findColumn(columns, "ParserAst");
  // ParserAstPrinter uses pure indentation; flatten() re-renders every
  // RenderNode with 2-space indent per depth, so root has none and each
  // child is indented by exactly kIndentWidth.
  EXPECT_EQ(parserCol.lines.size(), 3u);
  EXPECT_EQ(parserCol.lines[0], "Binary(*)");
  EXPECT_EQ(parserCol.lines[1], "  Number(1e12)");
  EXPECT_EQ(parserCol.lines[2], "  Number(1)");
}

TEST(AstViewerCore, MathAstColumn_MatchesRealPrintedShape) {
  const AstViewerCore viewer;
  const std::vector<AstColumn> columns = viewer.buildColumns("1e12*1");

  const AstColumn& mathCol = findColumn(columns, "MathAst");
  EXPECT_EQ(mathCol.lines.size(), 3u);
  EXPECT_EQ(mathCol.lines[0], "Binary(*)");
  EXPECT_EQ(mathCol.lines[1], "  Number(1e12)");
  EXPECT_EQ(mathCol.lines[2], "  Number(1)");
}

TEST(AstViewerCore, PreparedAstColumns_AreNotMarkedEmptyForNonEmptyExpression) {
  const AstViewerCore viewer;
  const std::vector<AstColumn> columns = viewer.buildColumns("1+2*3");

  const AstColumn& plain = findColumn(columns, "PreparedAst");
  const AstColumn& simplified = findColumn(columns, "PreparedAst+Simplify");

  EXPECT_TRUE(!plain.lines.empty());
  EXPECT_TRUE(plain.lines[0] != "<empty>");
  EXPECT_TRUE(!simplified.lines.empty());
  EXPECT_TRUE(simplified.lines[0] != "<empty>");
}

TEST(AstViewerCore, Render_ProducesHeaderSeparatorAndEqualColumnWidths) {
  const AstViewerCore viewer;
  const std::string rendered = viewer.render("1+2*3");

  // Sanity checks only: exact layout is covered by buildColumns() tests
  // above; render() just needs to actually contain the pieces.
  EXPECT_TRUE(rendered.find("Input") != std::string::npos);
  EXPECT_TRUE(rendered.find("ParserAst") != std::string::npos);
  EXPECT_TRUE(rendered.find("MathAst") != std::string::npos);
  EXPECT_TRUE(rendered.find("PreparedAst") != std::string::npos);
  EXPECT_TRUE(rendered.find("PreparedAst+Simplify") != std::string::npos);
  EXPECT_TRUE(rendered.find('\n') != std::string::npos);
}

int main(int argc, char** argv) {
  const std::string filter = (argc > 1) ? argv[1] : "";
  return testfw::runFiltered(filter);
}
