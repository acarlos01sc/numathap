#pragma once

#include <optional>
#include <vector>

#include "numathap/math/MathAst.hpp"
#include "numathap/math/MathNode.hpp"

namespace numathap::symbolic {

using namespace numathap::math;

/**
 * @brief Aggressive symbolic simplifier and expression normalizer.
 *
 * UltraSimplifier performs algebraic transformations intended primarily
 * to control AST growth during repeated symbolic differentiation.
 *
 * The normalization target is a sum of products of powers:
 *
 *     sum(term)
 *     term = factor * factor * ...
 *     factor = base ^ exponent
 *
 * Supported normalizations include:
 *
 *     A / B             -> A * B^(-1)
 *     (A + B) / C       -> A*C^(-1) + B*C^(-1)
 *     (A - B) / C       -> A*C^(-1) - B*C^(-1)
 *     A / (B + C)       -> A*(B + C)^(-1)
 *     sqrt(X)           -> X^(1/2)
 *     cbrt(X)           -> X^(1/3)
 *     A^p * A^q         -> A^(p + q)
 *     A * A^q           -> A^(1 + q)
 *     A / A^q           -> A^(1 - q)
 *
 * Numeric rational exponents are combined exactly.
 *
 * The simplifier intentionally does not perform general algebraic
 * expansion such as:
 *
 *     (A + B)(C + D) -> AC + AD + BC + BD
 *
 * nor transformations such as:
 *
 *     (A^p)^q -> A^(p*q)
 *
 * unless explicitly added in a future version.
 */
class UltraSimplifier {
   public:
    /**
     * @brief Simplify and normalize an AST.
     *
     * @param ast Input mathematical AST.
     * @return Simplified and normalized AST.
     */
    MathAst simplify(const MathAst& ast) const;

   private:
    // ---------------------------------------------------------------------
    // Rational arithmetic
    // ---------------------------------------------------------------------

    /**
     * @brief Exact rational number used internally for exponents.
     *
     * denominator is always positive and the fraction is kept reduced.
     */
    struct Rational {
        long long numerator{0};
        long long denominator{1};

        Rational() = default;

        explicit Rational(long long value) : numerator(value), denominator(1) {}

        Rational(long long numerator, long long denominator);

        void normalize();

        bool isZero() const noexcept;
        bool isOne() const noexcept;
        bool isNegative() const noexcept;

        Rational operator+(const Rational& other) const;
        Rational operator-(const Rational& other) const;
        Rational operator-() const;

        bool operator==(const Rational& other) const noexcept;
    };

    /**
     * @brief Try to interpret an AST node as an exact rational number.
     *
     * Recognized forms:
     *
     *     Number(n)       -> n/1
     *     Number(p) /
     *     Number(q)       -> p/q
     *
     * Only numeric numerator and denominator nodes are accepted.
     */
    std::optional<Rational> extractRational(const MathNode& node) const;

    /**
     * @brief Build an AST node representing an exact rational number.
     *
     * Examples:
     *
     *     2   -> Number(2)
     *     1/2 -> Binary(/, Number(1), Number(2))
     *     5/6 -> Binary(/, Number(5), Number(6))
     */
    MathNodePtr buildRational(const Rational& value) const;

    // ---------------------------------------------------------------------
    // Recursive normalization
    // ---------------------------------------------------------------------

    MathNodePtr simplifyNode(const MathNode& node) const;

    MathNodePtr simplifyBinary(const BinaryNode& node) const;

    MathNodePtr simplifyFunction(const FunctionNode& node) const;

    // ---------------------------------------------------------------------
    // Addition / subtraction
    // ---------------------------------------------------------------------

    MathNodePtr normalizeAdd(MathNodePtr lhs, MathNodePtr rhs) const;

    MathNodePtr normalizeSubtract(MathNodePtr lhs, MathNodePtr rhs) const;

    // ---------------------------------------------------------------------
    // Multiplication / division
    // ---------------------------------------------------------------------

    MathNodePtr normalizeMultiply(MathNodePtr lhs, MathNodePtr rhs) const;

    MathNodePtr normalizeDivide(MathNodePtr lhs, MathNodePtr rhs) const;

    /**
     * @brief Normalize division where the numerator is a sum/subtraction.
     *
     *     (A + B) / C -> A*C^(-1) + B*C^(-1)
     *     (A - B) / C -> A*C^(-1) - B*C^(-1)
     */
    MathNodePtr distributeDivisionOverNumerator(MathNodePtr numerator,
                                                MathNodePtr denominator) const;

    /**
     * @brief Build the multiplicative inverse of a node.
     *
     *     X -> X^(-1)
     */
    MathNodePtr buildInverse(MathNodePtr node) const;

    // ---------------------------------------------------------------------
    // Powers
    // ---------------------------------------------------------------------

    MathNodePtr normalizePower(MathNodePtr base, MathNodePtr exponent) const;

    /**
     * @brief Combine powers having structurally equivalent bases.
     *
     *     A^p * A^q -> A^(p+q)
     *
     * Exponents must be exact numeric rationals.
     */
    MathNodePtr combinePowers(MathNodePtr lhs, MathNodePtr rhs) const;

    /**
     * @brief Combine all factors with equivalent bases.
     *
     * Example:
     *
     *     A^2 * B * A^(1/3) * A^(-1)
     *
     * becomes:
     *
     *     A^(4/3) * B
     */
    MathNodePtr combinePowerFactors(std::vector<MathNodePtr> factors) const;

    // ---------------------------------------------------------------------
    // Factor collection
    // ---------------------------------------------------------------------

    /**
     * @brief Collect multiplicative factors recursively.
     *
     *     A * (B * C) -> [A, B, C]
     */
    void collectFactors(const MathNode& node,
                        std::vector<MathNodePtr>& factors) const;

    /**
     * @brief Build a multiplication tree from factors.
     */
    MathNodePtr buildProduct(std::vector<MathNodePtr> factors) const;

    // ---------------------------------------------------------------------
    // Sum collection
    // ---------------------------------------------------------------------

    /**
     * @brief Collect additive terms recursively.
     *
     *     A + (B + C) -> [A, B, C]
     *
     * Subtraction is preserved through the sign of the corresponding term.
     */
    void collectTerms(const MathNode& node,
                      std::vector<MathNodePtr>& terms) const;

    /**
     * @brief Build an addition tree from terms.
     */
    MathNodePtr buildSum(std::vector<MathNodePtr> terms) const;

    // ---------------------------------------------------------------------
    // Factor / power inspection
    // ---------------------------------------------------------------------

    /**
     * @brief Extract base and exponent from a factor.
     *
     *     A       -> base=A, exponent=1
     *     A^p     -> base=A, exponent=p
     *
     * Returns std::nullopt when the exponent is not a numeric rational.
     */
    struct PowerFactor {
        MathNodePtr base;
        Rational exponent;
    };

    std::optional<PowerFactor> extractPowerFactor(const MathNode& node) const;

    /**
     * @brief Build a power factor.
     *
     *     exponent == 0 -> 1
     *     exponent == 1 -> base
     *     otherwise      -> base^exponent
     */
    MathNodePtr buildPowerFactor(MathNodePtr base,
                                 const Rational& exponent) const;

    // ---------------------------------------------------------------------
    // Function normalization
    // ---------------------------------------------------------------------

    /**
     * @brief Normalize selected mathematical functions.
     *
     *     sqrt(X) -> X^(1/2)
     *     cbrt(X) -> X^(1/3)
     *
     * Other functions are preserved.
     */
    MathNodePtr normalizeFunction(const FunctionNode& node) const;

    // ---------------------------------------------------------------------
    // Structural utilities
    // ---------------------------------------------------------------------

    /**
     * @brief Check whether two nodes are structurally equivalent.
     */
    bool equivalent(const MathNode& lhs, const MathNode& rhs) const;

    /**
     * @brief Check whether a node represents numeric zero.
     */
    bool isZero(const MathNode& node) const;

    /**
     * @brief Check whether a node represents numeric one.
     */
    bool isOne(const MathNode& node) const;
};

}  // namespace numathap::symbolic