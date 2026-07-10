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
     * @brief Prints a Math-AST.
     *
     * @param os Output stream.
     * @param node Root node of the Math-AST.
     */
    void print(std::ostream& os,
               const MathNodePtr& node) const;

   private:
    /**
     * @brief Recursively prints a Math-AST node.
     *
     * @param os Output stream.
     * @param node Current node.
     * @param prefix Current indentation prefix.
     * @param isLast Indicates whether this node is the last child of
     *               its parent.
     */
    void printNode(std::ostream& os,
                   const MathNode& node,
                   const std::string& prefix,
                   bool isLast) const;
};

}  // namespace numathap::math