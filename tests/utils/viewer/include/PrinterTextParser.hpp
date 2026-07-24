#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace numathap::viewer {

struct RenderNode {
    std::string label;
    std::vector<RenderNode> children;
};

/**
 * @brief Parses tree-printer text output into a RenderNode tree.
 *
 * Supports two printer conventions found in numathap today:
 *  - Pure indentation (ParserAstPrinter): each depth level adds a fixed,
 *    auto-detected number of leading spaces, no connector glyphs.
 *  - Box-drawing connectors (MathAstPrinter): each line's prefix is made
 *    of 4-character blocks ("    ", "|   ", "|-- ", "`-- "), with the
 *    root line carrying no prefix at all.
 *
 * The style is auto-detected once per parse() call, from the presence
 * (or absence) of connector glyphs anywhere in the text -- a single
 * printer's output never mixes the two conventions.
 */
class PrinterTextParser {
   public:
    [[nodiscard]] RenderNode parse(const std::string& text) const;

   private:
    struct Line {
        int depth;
        std::string label;
    };

    [[nodiscard]] std::vector<Line> toLines(const std::string& text) const;
    [[nodiscard]] std::vector<Line> parseIndentStyle(
        const std::vector<std::string>& rawLines) const;
    [[nodiscard]] std::vector<Line> parseConnectorStyle(
        const std::vector<std::string>& rawLines) const;
    [[nodiscard]] int detectIndentUnit(const std::vector<int>& leadingSpaces) const;
    [[nodiscard]] RenderNode parseNode(const std::vector<Line>& lines,
                                        std::size_t& index, int depth) const;
};

} // namespace numathap::viewer