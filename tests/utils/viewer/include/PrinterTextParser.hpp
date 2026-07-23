/**
 * @file PrinterTextParser.hpp
 * @brief Reconstructs a generic tree from the textual output of an
 *        indentation-based AST printer (e.g. ParserAstPrinter).
 *
 * This component belongs to the viewer test utility (numathap/tests/utils/viewer).
 * It intentionally never includes or depends on numathap's public API
 * (prepare()/evaluate()) nor on any internal AST node type (Node, BinaryNode,
 * etc.). Its only input is the plain text produced by a tree's print()
 * method, which keeps the viewer fully decoupled from numathap's internal
 * representation, as required by the viewer's design.
 */
#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace numathap::viewer {

/**
 * @brief Generic, printer-agnostic representation of a printed tree node.
 *
 * RenderNode carries no semantic information about numathap's AST types.
 * It only stores the label text of a node (exactly as printed) and its
 * children, in printed order.
 */
struct RenderNode {
    std::string label;
    std::vector<RenderNode> children;
};

/**
 * @brief Parses indentation-based tree text into a RenderNode tree.
 *
 * Expected input format: one node per line, each line's label preceded
 * by a run of leading spaces proportional to its depth in the tree, as
 * produced by ParserAstPrinter and any other printer following the same
 * pure-indentation convention (no connector glyphs such as "|--"/"`--"
 * are expected or required).
 *
 * The indentation width (number of characters per depth level) is
 * inferred automatically from the text itself; this parser does not
 * hardcode any assumption about how many spaces a given printer uses
 * per level. This is possible because, in a pre-order printed traversal,
 * depth can only ever increase by exactly one from one line to the next
 * (descending into a first child); it can decrease by any amount when
 * returning to an ancestor, or stay the same for a sibling. Therefore the
 * smallest positive difference between the leading-space counts of
 * consecutive lines equals the printer's indentation unit.
 */
class PrinterTextParser {
   public:
    /**
     * @brief Parses the full textual output of a tree printer.
     *
     * @param text Text produced by calling a tree's print() method
     *             (e.g. ParserAst::print(os).str()).
     * @return The reconstructed RenderNode tree.
     *
     * @throws std::invalid_argument if the text is empty/blank, if a
     *         line's indentation does not align to the detected
     *         indentation unit, or if the text contains more than one
     *         root-level node.
     */
    [[nodiscard]] RenderNode parse(const std::string& text) const;

   private:
    struct Line {
        int depth;
        std::string label;
    };

    [[nodiscard]] std::vector<Line> toLines(const std::string& text) const;
    [[nodiscard]] int detectIndentUnit(const std::vector<int>& leadingSpaces) const;
    [[nodiscard]] RenderNode parseNode(const std::vector<Line>& lines,
                                        std::size_t& index, int depth) const;
};

} // namespace numathap::viewer