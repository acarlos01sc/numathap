/**
 * @file ParserAstPrinter.hpp
 * @brief Utility for formatting and visualizing the Abstract Syntax Tree (AST).
 * @see Parser.hpp
 */
#pragma once

#include <ostream>

#include "ParserNode.hpp"

namespace numathap::parser {

/**
 * @brief Prints a Parser AST tree.
 *
 * This class is intended for debugging and visualization of the
 * syntactic AST produced by Parser.
 */
class ParserAstPrinter {
   public:
    /**
     * @brief Prints a complete AST.
     */
    void print(const Node* node, std::ostream& os) const;

   private:
    void printNode(const Node* node, std::ostream& os, int depth) const;

    void indent(std::ostream& os, int depth) const;

    void printUnary(const UnaryNode* node, std::ostream& os, int depth) const;

    void printBinary(const BinaryNode* node, std::ostream& os, int depth) const;

    void printPostfix(const PostfixNode* node, std::ostream& os,
                      int depth) const;

    void printFunction(const FunctionCallNode* node, std::ostream& os,
                       int depth) const;

    void printAbsolute(const AbsoluteNode* node, std::ostream& os,
                       int depth) const;

    const char* unaryName(UnaryOp op) const;
    const char* binaryName(BinaryOp op) const;
    const char* postfixName(PostfixOp op) const;
};

}  // namespace numathap::parser