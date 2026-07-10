#pragma once

#include <ostream>
#include <string>

#include "numathap/parser/ast/Node.hpp"

namespace numathap::parser {

/**
 * @brief Pretty printer for the Parser Abstract Syntax Tree.
 *
 * AstPrinter traverses a Parser-AST and generates a human-readable
 * ASCII representation of its structure.
 *
 * The output is intended for debugging, testing and educational
 * purposes, allowing users to inspect how an expression has been
 * interpreted by the parser.
 */
class AstPrinter {
   public:
    AstPrinter() = default;

    /**
     * @brief Returns the AST as a formatted string.
     */
    [[nodiscard]]
    std::string toString(const ast::Node& node) const;

    [[nodiscard]]
    std::string toString(const ast::NodePtr& node) const;

    /**
     * @brief Prints the AST to an output stream.
     */
    void print(std::ostream& os, const ast::Node& node) const;

    void print(std::ostream& os, const ast::NodePtr& node) const;

   private:
    void printNode(std::ostream& os,
                   const ast::Node& node,
                   const std::string& prefix,
                   bool isLast) const;

    static std::string unaryOpName(ast::UnaryOp op);

    static std::string binaryOpName(ast::BinaryOp op);

    static std::string postfixOpName(ast::PostfixOp op);
};

} // namespace numathap::parser