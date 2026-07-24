#include "AstViewerCore.hpp"

#include <algorithm>
#include <sstream>

#include "numathap/config/Capability.hpp"
#include "numathap/config/MathEnvironment.hpp"
#include "numathap/math/MathAst.hpp"
#include "numathap/math/PreparedAst.hpp"
#include "numathap/parser/ParserAst.hpp"

namespace numathap::viewer {

namespace {

// Two spaces per depth level, independent of whichever indentation/connector
// convention the original printer used — RenderNode already normalized that
// away, so every column is rendered in one consistent visual style.
constexpr int kIndentWidth = 2;

bool isBlank(const std::string& text) {
  return text.find_first_not_of(" \t\r\n") == std::string::npos;
}

std::string padRight(std::string s, std::size_t width) {
  if (s.size() < width) {
    s.append(width - s.size(), ' ');
  }
  return s;
}

}  // namespace

void AstViewerCore::flatten(const RenderNode& node, int depth,
                             std::vector<std::string>& outLines) {
  outLines.push_back(std::string(static_cast<std::size_t>(depth) * kIndentWidth,
                                  ' ') +
                      node.label);
  for (const auto& child : node.children) {
    flatten(child, depth + 1, outLines);
  }
}

AstColumn AstViewerCore::buildInputColumn(const std::string& expression) const {
  AstColumn column{"Input", {}};
  column.lines.push_back(isBlank(expression) ? "<empty>" : expression);
  return column;
}

AstColumn AstViewerCore::buildTreeColumn(const std::string& title,
                                          const std::string& printedText) const {
  AstColumn column{title, {}};
  if (isBlank(printedText)) {
    column.lines.push_back("<empty>");
    return column;
  }

  PrinterTextParser parser;
  const RenderNode root = parser.parse(printedText);
  flatten(root, 0, column.lines);
  return column;
}

std::vector<AstColumn> AstViewerCore::buildColumns(
    const std::string& expression) const {
  std::vector<AstColumn> columns;
  columns.push_back(buildInputColumn(expression));

  numathap::parser::ParserAst parserAst(expression);
  {
    std::ostringstream oss;
    parserAst.print(oss);
    columns.push_back(buildTreeColumn("ParserAst", oss.str()));
  }

  numathap::math::MathAst mathAst(parserAst);
  {
    std::ostringstream oss;
    mathAst.print(oss);
    columns.push_back(buildTreeColumn("MathAst", oss.str()));
  }

  numathap::config::MathEnvironment envPlain;
  numathap::math::PreparedAst preparedPlain(mathAst, envPlain);
  {
    std::ostringstream oss;
    preparedPlain.print(oss);
    columns.push_back(buildTreeColumn("PreparedAst", oss.str()));
  }

  numathap::config::MathEnvironment envSimplify;
  envSimplify.enableCapability(numathap::config::Capability::Simplify);
  numathap::math::PreparedAst preparedSimplify(mathAst, envSimplify);
  {
    std::ostringstream oss;
    preparedSimplify.print(oss);
    columns.push_back(buildTreeColumn("PreparedAst+Simplify", oss.str()));
  }

  return columns;
}

std::string AstViewerCore::render(const std::string& expression) const {
  const std::vector<AstColumn> columns = buildColumns(expression);

  std::vector<std::size_t> widths;
  widths.reserve(columns.size());
  std::size_t maxHeight = 0;
  for (const auto& col : columns) {
    std::size_t width = col.title.size();
    for (const auto& line : col.lines) {
      width = std::max(width, line.size());
    }
    widths.push_back(width);
    maxHeight = std::max(maxHeight, col.lines.size());
  }

  std::ostringstream out;

  for (std::size_t i = 0; i < columns.size(); ++i) {
    if (i > 0) {
      out << " | ";
    }
    out << padRight(columns[i].title, widths[i]);
  }
  out << '\n';

  for (std::size_t i = 0; i < columns.size(); ++i) {
    if (i > 0) {
      out << "-+-";
    }
    out << std::string(widths[i], '-');
  }
  out << '\n';

  for (std::size_t row = 0; row < maxHeight; ++row) {
    for (std::size_t i = 0; i < columns.size(); ++i) {
      if (i > 0) {
        out << " | ";
      }
      const std::string cell =
          row < columns[i].lines.size() ? columns[i].lines[row] : "";
      out << padRight(cell, widths[i]);
    }
    out << '\n';
  }

  return out.str();
}

}  // namespace numathap::viewer
