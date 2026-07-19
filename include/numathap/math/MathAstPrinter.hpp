/**
 * @file MathAstPrinter.hpp
 * @brief Definition of the MathAstPrinter class.
 *
 * This file provides the utility for visualizing the semantic structure
 * of a Mathematical Abstract Syntax Tree (Math-AST).
 */
#pragma once

#include <ostream>
#include <string>

#include "numathap/math/MathNode.hpp"

namespace numathap::math {

/**
 * @brief Pretty printer for the Mathematical Abstract Syntax Tree
 * (Math-AST).
 *
 * The MathAstPrinter traverses a Math-AST and produces a human-readable
 * tree representation intended for debugging and development.
 *
 * Unlike the parser AST printer, this printer displays the semantic
 * structure produced by the MathAstBuilder.
 */
class MathAstPrinter {
   public:
    /**
     * @brief Prints a Math-AST to the specified output stream.
     *
     * @param node The root node of the Math-AST to print.
     * @param os The output stream where the tree will be displayed.
     */
    void print(const MathNode& node, std::ostream& os) const;

   private:
    /**
     * @brief Recursively prints a Math-AST node.
     *
     * Traverses the node hierarchy and formats the output with appropriate
     * indentation and structure markers.
     *
     * @param os The output stream.
     * @param node The current node to print.
     * @param prefix The current indentation string prefix.
     * @param isLast A flag indicating whether this node is the last child
     *               of its parent.
     */
    void printNode(std::ostream& os, const MathNode& node,
                   const std::string& prefix, bool isLast) const;
};

}  // namespace numathap::math