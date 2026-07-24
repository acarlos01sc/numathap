/**
 * @file AstViewerCore.hpp
 * @brief Core engine of the AST inspection viewer.
 *
 * Given a mathematical expression, AstViewerCore instantiates the four
 * trees the viewer is meant to compare (ParserAst, MathAst, PreparedAst
 * without Capability, PreparedAst with Capability::Simplify), captures
 * each tree's print() output, reconstructs a generic RenderNode tree via
 * PrinterTextParser, and lays the result out as side-by-side columns.
 *
 * This component only ever touches numathap through each tree's print()
 * method (via PrinterTextParser) — it never inspects numathap's internal
 * node types directly, matching the viewer's decoupling requirement.
 *
 * No YAML/snapshot support yet: this is only the central engine that
 * produces the column layout for a single expression.
 */
#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "PrinterTextParser.hpp"

namespace numathap::viewer {

/**
 * @brief One column of the side-by-side layout.
 *
 * `lines` already reflects the final visual form of the column (one
 * entry per printed row); `<empty>` is represented as a single-element
 * lines vector containing exactly that placeholder, so callers never
 * need to special-case emptiness themselves.
 */
struct AstColumn {
  std::string title;
  std::vector<std::string> lines;
};

/**
 * @brief Builds the side-by-side AST comparison for a single expression.
 */
class AstViewerCore {
 public:
  /**
   * @brief Builds the five columns for `expression`:
   *        Input, ParserAst, MathAst, PreparedAst, PreparedAst+Simplify.
   *
   * @param expression Expression to parse and prepare.
   * @throws std::invalid_argument if any printed tree text is malformed
   *         (propagated from PrinterTextParser::parse).
   */
  [[nodiscard]] std::vector<AstColumn> buildColumns(
      const std::string& expression) const;

  /**
   * @brief Renders `buildColumns(expression)` as a single text block with
   *        automatic column widths, a title row, and a separator row.
   */
  [[nodiscard]] std::string render(const std::string& expression) const;

 private:
  [[nodiscard]] AstColumn buildInputColumn(const std::string& expression) const;
  [[nodiscard]] AstColumn buildTreeColumn(const std::string& title,
                                          const std::string& printedText) const;

  static void flatten(const RenderNode& node, int depth,
                       std::vector<std::string>& outLines);
};

}  // namespace numathap::viewer
