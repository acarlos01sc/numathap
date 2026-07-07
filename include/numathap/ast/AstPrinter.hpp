#pragma once

#include <iosfwd>
#include <string>

#include "numathap/ast/Node.hpp"

namespace numathap::ast {

/**
 * @brief Utility class for printing abstract syntax trees.
 *
 * AstPrinter traverses an AST and produces a human-readable
 * tree representation useful for debugging and parser validation.
 */
class AstPrinter {
   public:
    /**
     * @brief Prints an abstract syntax tree.
     *
     * @param node Root node of the AST.
     * @param out Output stream where the tree is written.
     */
    static void print(const Node& node, std::ostream& out);

   private:
    /**
     * @brief Recursively prints an AST node.
     *
     * @param node Current AST node.
     * @param out Output stream.
     * @param prefix Current indentation prefix.
     * @param isLast Indicates whether the node is the last child.
     */
    static void printNode(const Node& node,
                          std::ostream& out,
                          const std::string& prefix,
                          bool isLast);
};

}  // namespace numathap::ast