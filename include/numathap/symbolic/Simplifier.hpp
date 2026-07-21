#pragma once

#include "numathap/math/MathAst.hpp"
#include "numathap/math/MathNode.hpp"

namespace numathap::symbolic {

class Simplifier {
public:
    [[nodiscard]]
    math::MathAst simplify(const math::MathAst& mathAst) const;

private:
    [[nodiscard]]
    math::MathNodePtr simplifyNode(const math::MathNode& node) const;

    [[nodiscard]]
    math::MathNodePtr simplifyNumber(const math::NumberNode& node) const;

    [[nodiscard]]
    math::MathNodePtr simplifySymbol(const math::SymbolNode& node) const;

    [[nodiscard]]
    math::MathNodePtr simplifyUnary(const math::UnaryNode& node) const;

    [[nodiscard]]
    math::MathNodePtr simplifyBinary(const math::BinaryNode& node) const;

    [[nodiscard]]
    math::MathNodePtr simplifyFunction(const math::FunctionNode& node) const;
};

} // namespace numathap::symbolic