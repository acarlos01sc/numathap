/**
 * @file UltraSimplifier.cpp
 * @brief Implements the UltraSimplifier.
 */

#include "numathap/symbolic/UltraSimplifier.hpp"

#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "numathap/backend/BackendSupport.hpp"
#include "numathap/core/Value.hpp"
#include "numathap/dispatch/Dispatcher.hpp"

namespace numathap::symbolic {

using namespace numathap::math;
using namespace numathap::backend;

// ============================================================================
// Rational
// ============================================================================

UltraSimplifier::Rational::Rational(long long numerator,
                                     long long denominator)
    : numerator(numerator), denominator(denominator) {
    normalize();
}

void UltraSimplifier::Rational::normalize() {
    if (denominator == 0) {
        throw std::invalid_argument(
            "UltraSimplifier: rational denominator cannot be zero.");
    }

    if (denominator < 0) {
        numerator = -numerator;
        denominator = -denominator;
    }

    auto gcd = [](long long a, long long b) {
        if (a < 0) {
            a = -a;
        }

        if (b < 0) {
            b = -b;
        }

        while (b != 0) {
            const long long r = a % b;
            a = b;
            b = r;
        }

        return a;
    };

    const long long divisor = gcd(numerator, denominator);

    if (divisor != 0) {
        numerator /= divisor;
        denominator /= divisor;
    }
}

bool UltraSimplifier::Rational::isZero() const noexcept {
    return numerator == 0;
}

bool UltraSimplifier::Rational::isOne() const noexcept {
    return numerator == denominator;
}

bool UltraSimplifier::Rational::isNegative() const noexcept {
    return numerator < 0;
}

UltraSimplifier::Rational UltraSimplifier::Rational::operator+(
    const Rational& other) const {
    return Rational(
        numerator * other.denominator +
            other.numerator * denominator,
        denominator * other.denominator);
}

UltraSimplifier::Rational UltraSimplifier::Rational::operator-(
    const Rational& other) const {
    return Rational(
        numerator * other.denominator -
            other.numerator * denominator,
        denominator * other.denominator);
}

UltraSimplifier::Rational UltraSimplifier::Rational::operator-() const {
    return Rational(-numerator, denominator);
}

bool UltraSimplifier::Rational::operator==(
    const Rational& other) const noexcept {
    return numerator == other.numerator &&
           denominator == other.denominator;
}

// ============================================================================
// Public entry point
// ============================================================================

MathAst UltraSimplifier::simplify(const MathAst& ast) const {
    if (ast.root() == nullptr) {
        return MathAst(ast.expression(), nullptr);
    }

    return MathAst(ast.expression(), simplifyNode(*ast.root()));
}

// ============================================================================
// Recursive normalization
// ============================================================================

MathNodePtr UltraSimplifier::simplifyNode(const MathNode& node) const {
    return dispatch::Dispatcher::dispatch(
        node,
        [this](const auto& concreteNode) -> MathNodePtr {
            using NodeType = std::decay_t<decltype(concreteNode)>;

            if constexpr (std::is_same_v<NodeType, NumberNode>) {
                return std::make_unique<NumberNode>(concreteNode.value);

            } else if constexpr (std::is_same_v<NodeType, SymbolNode>) {
                return std::make_unique<SymbolNode>(concreteNode.name);

            } else if constexpr (std::is_same_v<NodeType, UnaryNode>) {
                auto operand =
                    simplifyNode(*concreteNode.operand);

                if (concreteNode.op == UnaryOp::Plus) {
                    return operand;
                }

                return std::make_unique<UnaryNode>(
                    concreteNode.op,
                    std::move(operand));

            } else if constexpr (std::is_same_v<NodeType, BinaryNode>) {
                return simplifyBinary(concreteNode);

            } else if constexpr (std::is_same_v<NodeType, FunctionNode>) {
                return simplifyFunction(concreteNode);

            } else {
                static_assert(
                    std::is_same_v<NodeType, void>,
                    "UltraSimplifier: unsupported MathNode type.");
            }
        });
}

MathNodePtr UltraSimplifier::simplifyBinary(
    const BinaryNode& node) const {
    auto lhs = simplifyNode(*node.left);
    auto rhs = simplifyNode(*node.right);

    switch (node.op) {
        case BinaryOp::Add:
            return normalizeAdd(
                std::move(lhs),
                std::move(rhs));

        case BinaryOp::Subtract:
            return normalizeSubtract(
                std::move(lhs),
                std::move(rhs));

        case BinaryOp::Multiply:
            return normalizeMultiply(
                std::move(lhs),
                std::move(rhs));

        case BinaryOp::Divide:
            return normalizeDivide(
                std::move(lhs),
                std::move(rhs));

        case BinaryOp::Power:
            return normalizePower(
                std::move(lhs),
                std::move(rhs));
    }

    throw std::logic_error(
        "UltraSimplifier: unknown binary operator.");
}

MathNodePtr UltraSimplifier::simplifyFunction(
    const FunctionNode& node) const {
    return normalizeFunction(node);
}

// ============================================================================
// Rational extraction / construction
// ============================================================================

std::optional<UltraSimplifier::Rational>
UltraSimplifier::extractRational(
    const MathNode& node) const {
    return dispatch::Dispatcher::dispatch(
        node,
        [this](const auto& concreteNode)
            -> std::optional<Rational> {
            using NodeType =
                std::decay_t<decltype(concreteNode)>;

            if constexpr (std::is_same_v<NodeType, NumberNode>) {
                try {
                    std::size_t pos = 0;

                    const long long value =
                        std::stoll(
                            concreteNode.value,
                            &pos);

                    if (pos != concreteNode.value.size()) {
                        return std::nullopt;
                    }

                    return Rational(value);

                } catch (...) {
                    return std::nullopt;
                }

            } else if constexpr (
                std::is_same_v<NodeType, BinaryNode>) {

                if (concreteNode.op != BinaryOp::Divide) {
                    return std::nullopt;
                }

                const auto numerator =
                    extractRational(*concreteNode.left);

                const auto denominator =
                    extractRational(*concreteNode.right);

                if (!numerator ||
                    !denominator ||
                    denominator->isZero()) {
                    return std::nullopt;
                }

                return Rational(
                    numerator->numerator *
                        denominator->denominator,
                    numerator->denominator *
                        denominator->numerator);

            } else {
                return std::nullopt;
            }
        });
}

MathNodePtr UltraSimplifier::buildRational(
    const Rational& value) const {
    if (value.denominator == 1) {
        return std::make_unique<NumberNode>(
            std::to_string(value.numerator));
    }

    auto numerator =
        std::make_unique<NumberNode>(
            std::to_string(value.numerator));

    auto denominator =
        std::make_unique<NumberNode>(
            std::to_string(value.denominator));

    return std::make_unique<BinaryNode>(
        BinaryOp::Divide,
        std::move(numerator),
        std::move(denominator));
}

// ============================================================================
// Addition / subtraction
// ============================================================================

MathNodePtr UltraSimplifier::normalizeAdd(
    MathNodePtr lhs,
    MathNodePtr rhs) const {
    if (isZero(*lhs)) {
        return rhs;
    }

    if (isZero(*rhs)) {
        return lhs;
    }

    std::vector<MathNodePtr> terms;

    collectTerms(*lhs, terms);
    collectTerms(*rhs, terms);

    return buildSum(std::move(terms));
}

MathNodePtr UltraSimplifier::normalizeSubtract(
    MathNodePtr lhs,
    MathNodePtr rhs) const {
    if (isZero(*rhs)) {
        return lhs;
    }

    /*
     * Convert:
     *
     *     A - B
     *
     * into:
     *
     *     A + (-B)
     *
     * so that the same like-term machinery can be used.
     */
    std::vector<MathNodePtr> terms;

    collectTerms(*lhs, terms);

    terms.push_back(
        std::make_unique<UnaryNode>(
            UnaryOp::Minus,
            std::move(rhs)));

    return buildSum(std::move(terms));
}

// ============================================================================
// Multiplication
// ============================================================================

MathNodePtr UltraSimplifier::normalizeMultiply(
    MathNodePtr lhs,
    MathNodePtr rhs) const {
    if (isZero(*lhs) || isZero(*rhs)) {
        return std::make_unique<NumberNode>("0");
    }

    if (isOne(*lhs)) {
        return rhs;
    }

    if (isOne(*rhs)) {
        return lhs;
    }

    std::vector<MathNodePtr> factors;

    collectFactors(*lhs, factors);
    collectFactors(*rhs, factors);

    /*
     * This is the important reduction step:
     *
     *     A^p * A^q
     *
     * becomes
     *
     *     A^(p+q)
     *
     * and all equivalent factors are accumulated.
     */
    return combinePowerFactors(std::move(factors));
}

// ============================================================================
// Division
// ============================================================================

MathNodePtr UltraSimplifier::normalizeDivide(
    MathNodePtr lhs,
    MathNodePtr rhs) const {
    if (isZero(*lhs)) {
        return std::make_unique<BinaryNode>(
            BinaryOp::Divide,
            std::move(lhs),
            std::move(rhs));
    }

    if (isOne(*rhs)) {
        return lhs;
    }

    //
    // (A + B) / C
    // (A - B) / C
    //
    if (auto* binary =
            dynamic_cast<BinaryNode*>(lhs.get())) {

        if (binary->op == BinaryOp::Add ||
            binary->op == BinaryOp::Subtract) {

            return distributeDivisionOverNumerator(
                std::move(lhs),
                std::move(rhs));
        }
    }

    //
    // A / A^q -> A^(1-q)
    //
    auto numeratorFactor =
        extractPowerFactor(*lhs);

    auto denominatorFactor =
        extractPowerFactor(*rhs);

    if (numeratorFactor &&
        denominatorFactor &&
        equivalent(
            *numeratorFactor->base,
            *denominatorFactor->base)) {

        const Rational exponent =
            numeratorFactor->exponent -
            denominatorFactor->exponent;

        return buildPowerFactor(
            std::move(numeratorFactor->base),
            exponent);
    }

    //
    // General normalization:
    //
    // A / B -> A * B^(-1)
    //
    auto inverse =
        buildInverse(std::move(rhs));

    return normalizeMultiply(
        std::move(lhs),
        std::move(inverse));
}

MathNodePtr UltraSimplifier::distributeDivisionOverNumerator(
    MathNodePtr numerator,
    MathNodePtr denominator) const {
    auto* binary =
        dynamic_cast<BinaryNode*>(numerator.get());

    if (binary == nullptr) {
        return normalizeMultiply(
            std::move(numerator),
            buildInverse(std::move(denominator)));
    }

    const BinaryOp op = binary->op;

    auto left =
        std::move(binary->left);

    auto right =
        std::move(binary->right);

    auto denominatorInverse =
        buildInverse(std::move(denominator));

    auto leftTerm =
        normalizeMultiply(
            std::move(left),
            BackendSupport::cloneNode(
                *denominatorInverse));

    auto rightTerm =
        normalizeMultiply(
            std::move(right),
            std::move(denominatorInverse));

    if (op == BinaryOp::Add) {
        return normalizeAdd(
            std::move(leftTerm),
            std::move(rightTerm));
    }

    return normalizeSubtract(
        std::move(leftTerm),
        std::move(rightTerm));
}

MathNodePtr UltraSimplifier::buildInverse(
    MathNodePtr node) const {
    //
    // If X is already X^q, invert its exponent directly.
    //
    auto factor =
        extractPowerFactor(*node);

    if (factor) {
        return buildPowerFactor(
            std::move(factor->base),
            -factor->exponent);
    }

    return buildPowerFactor(
        std::move(node),
        Rational(-1));
}

// ============================================================================
// Powers
// ============================================================================

MathNodePtr UltraSimplifier::normalizePower(
    MathNodePtr base,
    MathNodePtr exponent) const {
    const auto rational =
        extractRational(*exponent);

    if (!rational) {
        return std::make_unique<BinaryNode>(
            BinaryOp::Power,
            std::move(base),
            std::move(exponent));
    }

    /*
     * Power of a power:
     *
     *     (A^p)^n -> A^(p*n)
     *
     * This transformation is intentionally restricted to a
     * positive integer outer exponent.
     *
     * The restriction avoids transformations such as:
     *
     *     (x^2)^(1/2) -> x
     *
     * which would not preserve the real-valued meaning because
     * the left-hand side is |x|.
     *
     * For a positive integer n, however:
     *
     *     (A^p)^n = A^(p*n)
     *
     * on the domain where A^p is defined.
     */
    if (rational->denominator == 1 &&
        rational->numerator > 0) {

        auto* innerPower =
            dynamic_cast<BinaryNode*>(base.get());

        if (innerPower != nullptr &&
            innerPower->op == BinaryOp::Power) {

            const auto innerExponent =
                extractRational(*innerPower->right);

            if (innerExponent) {
                const Rational combinedExponent(
                    innerExponent->numerator *
                        rational->numerator,
                    innerExponent->denominator);

                auto innerBase =
                    std::move(innerPower->left);

                return buildPowerFactor(
                    std::move(innerBase),
                    combinedExponent);
            }
        }
    }

    return buildPowerFactor(
        std::move(base),
        *rational);
}

MathNodePtr UltraSimplifier::combinePowers(
    MathNodePtr lhs,
    MathNodePtr rhs) const {
    auto leftFactor =
        extractPowerFactor(*lhs);

    auto rightFactor =
        extractPowerFactor(*rhs);

    if (!leftFactor || !rightFactor) {
        return std::make_unique<BinaryNode>(
            BinaryOp::Multiply,
            std::move(lhs),
            std::move(rhs));
    }

    if (!equivalent(
            *leftFactor->base,
            *rightFactor->base)) {

        return std::make_unique<BinaryNode>(
            BinaryOp::Multiply,
            std::move(lhs),
            std::move(rhs));
    }

    const Rational exponent =
        leftFactor->exponent +
        rightFactor->exponent;

    return buildPowerFactor(
        std::move(leftFactor->base),
        exponent);
}

MathNodePtr UltraSimplifier::combinePowerFactors(
    std::vector<MathNodePtr> factors) const {
    if (factors.empty()) {
        return std::make_unique<NumberNode>("1");
    }

    /*
     * Each entry represents one distinct structural base and
     * the accumulated rational exponent associated with it.
     */
    struct AccumulatedFactor {
        MathNodePtr base;
        Rational exponent;
    };

    std::vector<AccumulatedFactor> accumulated;
    std::vector<MathNodePtr> nonPowerFactors;

    for (auto& factor : factors) {
        auto powerFactor =
            extractPowerFactor(*factor);

        if (!powerFactor) {
            nonPowerFactors.push_back(
                std::move(factor));
            continue;
        }

        bool found = false;

        for (auto& existing : accumulated) {
            if (!equivalent(
                    *existing.base,
                    *powerFactor->base)) {
                continue;
            }

            /*
             *     A^p * A^q -> A^(p+q)
             */
            existing.exponent =
                existing.exponent +
                powerFactor->exponent;

            found = true;
            break;
        }

        if (!found) {
            accumulated.push_back(
                AccumulatedFactor{
                    std::move(powerFactor->base),
                    powerFactor->exponent});
        }
    }

    std::vector<MathNodePtr> result;

    result.reserve(
        nonPowerFactors.size() +
        accumulated.size());

    //
    // Preserve factors whose exponents could not
    // be interpreted as exact rationals.
    //
    for (auto& factor : nonPowerFactors) {
        if (!isOne(*factor)) {
            result.push_back(
                std::move(factor));
        }
    }

    //
    // Rebuild all accumulated powers.
    //
    for (auto& entry : accumulated) {
        /*
         * A^0 -> 1
         */
        if (entry.exponent.isZero()) {
            continue;
        }

        result.push_back(
            buildPowerFactor(
                std::move(entry.base),
                entry.exponent));
    }

    return buildProduct(std::move(result));
}

// ============================================================================
// Factor collection
// ============================================================================

void UltraSimplifier::collectFactors(
    const MathNode& node,
    std::vector<MathNodePtr>& factors) const {
    const auto* binary =
        dynamic_cast<const BinaryNode*>(&node);

    if (binary != nullptr &&
        binary->op == BinaryOp::Multiply) {

        collectFactors(
            *binary->left,
            factors);

        collectFactors(
            *binary->right,
            factors);

        return;
    }

    factors.push_back(
        simplifyNode(node));
}

MathNodePtr UltraSimplifier::buildProduct(
    std::vector<MathNodePtr> factors) const {
    if (factors.empty()) {
        return std::make_unique<NumberNode>("1");
    }

    std::vector<MathNodePtr> filtered;

    filtered.reserve(factors.size());

    for (auto& factor : factors) {
        if (isZero(*factor)) {
            return std::make_unique<NumberNode>("0");
        }

        if (!isOne(*factor)) {
            filtered.push_back(
                std::move(factor));
        }
    }

    if (filtered.empty()) {
        return std::make_unique<NumberNode>("1");
    }

    if (filtered.size() == 1) {
        return std::move(filtered.front());
    }

    MathNodePtr result =
        std::move(filtered.front());

    for (std::size_t i = 1;
         i < filtered.size();
         ++i) {

        result =
            std::make_unique<BinaryNode>(
                BinaryOp::Multiply,
                std::move(result),
                std::move(filtered[i]));
    }

    return result;
}

// ============================================================================
// Sum collection
// ============================================================================

void UltraSimplifier::collectTerms(
    const MathNode& node,
    std::vector<MathNodePtr>& terms) const {
    const auto* binary =
        dynamic_cast<const BinaryNode*>(&node);

    if (binary != nullptr &&
        binary->op == BinaryOp::Add) {

        collectTerms(
            *binary->left,
            terms);

        collectTerms(
            *binary->right,
            terms);

        return;
    }

    terms.push_back(
        simplifyNode(node));
}

MathNodePtr UltraSimplifier::buildSum(
    std::vector<MathNodePtr> terms) const {
    if (terms.empty()) {
        return std::make_unique<NumberNode>("0");
    }

    /*
     * A term is represented internally as:
     *
     *     coefficient * symbolicPart
     *
     * Examples:
     *
     *     x       -> 1 * x
     *     2*x     -> 2 * x
     *     3*x^2   -> 3 * x^2
     *
     * A null symbolicPart represents a pure numeric term.
     */
    struct LikeTerm {
        Rational coefficient;
        MathNodePtr symbolicPart;
    };

    /*
     * Rational multiplication is needed for products such as:
     *
     *     (2/3) * 3 * x
     *
     * Rational itself intentionally exposes only the operations
     * required by the existing exponent machinery, so the
     * multiplication is performed locally here.
     */
    auto multiplyRational =
        [](const Rational& lhs,
           const Rational& rhs) -> Rational {
            return Rational(
                lhs.numerator * rhs.numerator,
                lhs.denominator * rhs.denominator);
        };

    /*
     * Extract the numeric coefficient and the symbolic part
     * of a term.
     *
     * The lambda is recursive so that unary minus and products
     * can both contribute to the coefficient.
     */
    std::function<LikeTerm(const MathNode&)>
        extractLikeTerm;

    extractLikeTerm =
        [&](const MathNode& node) -> LikeTerm {
        //
        // Pure rational number.
        //
        if (auto rational = extractRational(node)) {
            return LikeTerm{
                *rational,
                nullptr};
        }

        //
        // Unary sign.
        //
        if (const auto* unary =
                dynamic_cast<const UnaryNode*>(&node)) {

            auto result =
                extractLikeTerm(*unary->operand);

            if (unary->op == UnaryOp::Minus) {
                result.coefficient =
                    -result.coefficient;
            }

            return result;
        }

        //
        // Product.
        //
        if (const auto* binary =
                dynamic_cast<const BinaryNode*>(&node)) {

            if (binary->op == BinaryOp::Multiply) {
                std::vector<MathNodePtr> factors;

                collectFactors(
                    node,
                    factors);

                Rational coefficient(1);
                std::vector<MathNodePtr> symbolicFactors;

                for (auto& factor : factors) {
                    auto factorTerm =
                        extractLikeTerm(*factor);

                    coefficient =
                        multiplyRational(
                            coefficient,
                            factorTerm.coefficient);

                    if (factorTerm.symbolicPart) {
                        symbolicFactors.push_back(
                            std::move(
                                factorTerm.symbolicPart));
                    }
                }

                if (symbolicFactors.empty()) {
                    return LikeTerm{
                        coefficient,
                        nullptr};
                }

                auto symbolicPart =
                    combinePowerFactors(
                        std::move(symbolicFactors));

                return LikeTerm{
                    coefficient,
                    std::move(symbolicPart)};
            }
        }

        //
        // Any other expression is a symbolic part with
        // coefficient 1.
        //
        return LikeTerm{
            Rational(1),
            simplifyNode(node)};
    };

    /*
     * Extract all terms.
     */
    std::vector<LikeTerm> extracted;
    extracted.reserve(terms.size());

    for (auto& term : terms) {
        extracted.push_back(
            extractLikeTerm(*term));
    }

    /*
     * Accumulate terms with structurally equivalent symbolic
     * parts.
     */
    struct AccumulatedTerm {
        Rational coefficient;
        MathNodePtr symbolicPart;
    };

    std::vector<AccumulatedTerm> accumulated;

    for (auto& term : extracted) {
        bool found = false;

        for (auto& existing : accumulated) {
            //
            // Both are pure numeric terms.
            //
            if (!existing.symbolicPart &&
                !term.symbolicPart) {

                existing.coefficient =
                    existing.coefficient +
                    term.coefficient;

                found = true;
                break;
            }

            //
            // One is numeric and the other is symbolic.
            //
            if (!existing.symbolicPart ||
                !term.symbolicPart) {
                continue;
            }

            //
            // Same symbolic part -> combine coefficients.
            //
            if (equivalent(
                    *existing.symbolicPart,
                    *term.symbolicPart)) {

                existing.coefficient =
                    existing.coefficient +
                    term.coefficient;

                found = true;
                break;
            }
        }

        if (!found) {
            accumulated.push_back(
                AccumulatedTerm{
                    term.coefficient,
                    std::move(term.symbolicPart)});
        }
    }

    /*
     * Rebuild the normalized sum.
     */
    std::vector<MathNodePtr> normalizedTerms;

    normalizedTerms.reserve(
        accumulated.size());

    for (auto& term : accumulated) {
        //
        // A zero coefficient removes the entire term.
        //
        if (term.coefficient.isZero()) {
            continue;
        }

        //
        // Pure numeric term.
        //
        if (!term.symbolicPart) {
            normalizedTerms.push_back(
                buildRational(
                    term.coefficient));

            continue;
        }

        //
        // coefficient == 1:
        //
        //     1*x -> x
        //
        if (term.coefficient.isOne()) {
            normalizedTerms.push_back(
                std::move(term.symbolicPart));

            continue;
        }

        //
        // coefficient == -1:
        //
        //     -1*x -> -x
        //
        if (term.coefficient.numerator == -1 &&
            term.coefficient.denominator == 1) {

            normalizedTerms.push_back(
                std::make_unique<UnaryNode>(
                    UnaryOp::Minus,
                    std::move(
                        term.symbolicPart)));

            continue;
        }

        //
        // General coefficient:
        //
        //     c*x
        //
        normalizedTerms.push_back(
            std::make_unique<BinaryNode>(
                BinaryOp::Multiply,
                buildRational(
                    term.coefficient),
                std::move(
                    term.symbolicPart)));
    }

    if (normalizedTerms.empty()) {
        return std::make_unique<NumberNode>("0");
    }

    if (normalizedTerms.size() == 1) {
        return std::move(
            normalizedTerms.front());
    }

    MathNodePtr result =
        std::move(normalizedTerms.front());

    for (std::size_t i = 1;
         i < normalizedTerms.size();
         ++i) {

        result =
            std::make_unique<BinaryNode>(
                BinaryOp::Add,
                std::move(result),
                std::move(normalizedTerms[i]));
    }

    return result;
}

// ============================================================================
// Factor / power inspection
// ============================================================================

std::optional<UltraSimplifier::PowerFactor>
UltraSimplifier::extractPowerFactor(
    const MathNode& node) const {
    const auto* binary =
        dynamic_cast<const BinaryNode*>(&node);

    if (binary != nullptr &&
        binary->op == BinaryOp::Power) {

        auto exponent =
            extractRational(*binary->right);

        if (!exponent) {
            return std::nullopt;
        }

        return PowerFactor{
            simplifyNode(*binary->left),
            *exponent};
    }

    //
    // Any non-power factor is interpreted as base^1.
    //
    return PowerFactor{
        simplifyNode(node),
        Rational(1)};
}

MathNodePtr UltraSimplifier::buildPowerFactor(
    MathNodePtr base,
    const Rational& exponent) const {
    if (exponent.isZero()) {
        return std::make_unique<NumberNode>("1");
    }

    if (exponent.isOne()) {
        return base;
    }

    auto exponentNode =
        buildRational(exponent);

    return std::make_unique<BinaryNode>(
        BinaryOp::Power,
        std::move(base),
        std::move(exponentNode));
}

// ============================================================================
// Function normalization
// ============================================================================

MathNodePtr UltraSimplifier::normalizeFunction(
    const FunctionNode& node) const {
    std::vector<MathNodePtr> arguments;

    arguments.reserve(
        node.arguments.size());

    for (const auto& argument :
         node.arguments) {

        arguments.push_back(
            simplifyNode(*argument));
    }

    //
    // sqrt(X) -> X^(1/2)
    //
    if (node.name == "sqrt" &&
        arguments.size() == 1) {

        return buildPowerFactor(
            std::move(arguments.front()),
            Rational(1, 2));
    }

    //
    // cbrt(X) -> X^(1/3)
    //
    if (node.name == "cbrt" &&
        arguments.size() == 1) {

        return buildPowerFactor(
            std::move(arguments.front()),
            Rational(1, 3));
    }

    return std::make_unique<FunctionNode>(
        node.name,
        std::move(arguments));
}

// ============================================================================
// Structural utilities
// ============================================================================

bool UltraSimplifier::equivalent(
    const MathNode& lhs,
    const MathNode& rhs) const {
    struct EquivalentVisitor {
        const UltraSimplifier* self;
        const MathNode& rhs;

        bool operator()(
            const NumberNode& left) const {
            const auto* right =
                dynamic_cast<const NumberNode*>(&rhs);

            return right != nullptr &&
                   left.value == right->value;
        }

        bool operator()(
            const SymbolNode& left) const {
            const auto* right =
                dynamic_cast<const SymbolNode*>(&rhs);

            return right != nullptr &&
                   left.name == right->name;
        }

        bool operator()(
            const UnaryNode& left) const {
            const auto* right =
                dynamic_cast<const UnaryNode*>(&rhs);

            return right != nullptr &&
                   left.op == right->op &&
                   self->equivalent(
                       *left.operand,
                       *right->operand);
        }

        bool operator()(
            const BinaryNode& left) const {
            const auto* right =
                dynamic_cast<const BinaryNode*>(&rhs);

            return right != nullptr &&
                   left.op == right->op &&
                   self->equivalent(
                       *left.left,
                       *right->left) &&
                   self->equivalent(
                       *left.right,
                       *right->right);
        }

        bool operator()(
            const FunctionNode& left) const {
            const auto* right =
                dynamic_cast<const FunctionNode*>(&rhs);

            if (right == nullptr ||
                left.name != right->name ||
                left.arguments.size() !=
                    right->arguments.size()) {

                return false;
            }

            for (std::size_t i = 0;
                 i < left.arguments.size();
                 ++i) {

                if (!self->equivalent(
                        *left.arguments[i],
                        *right->arguments[i])) {

                    return false;
                }
            }

            return true;
        }
    };

    return dispatch::Dispatcher::dispatch(
        lhs,
        EquivalentVisitor{this, rhs});
}

bool UltraSimplifier::isZero(
    const MathNode& node) const {
    return dispatch::Dispatcher::dispatch(
        node,
        [](const auto& concreteNode) -> bool {
            using NodeType =
                std::decay_t<decltype(concreteNode)>;

            if constexpr (
                std::is_same_v<NodeType, NumberNode>) {

                return core::Value::parse(
                           concreteNode.value) ==
                       core::Value::parse("0");

            } else {
                return false;
            }
        });
}

bool UltraSimplifier::isOne(
    const MathNode& node) const {
    return dispatch::Dispatcher::dispatch(
        node,
        [](const auto& concreteNode) -> bool {
            using NodeType =
                std::decay_t<decltype(concreteNode)>;

            if constexpr (
                std::is_same_v<NodeType, NumberNode>) {

                return core::Value::parse(
                           concreteNode.value) ==
                       core::Value::parse("1");

            } else {
                return false;
            }
        });
}

}  // namespace numathap::symbolic