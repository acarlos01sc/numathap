/**
 * @file differentiate_test.cpp
 * @brief Tests the symbolic differentiation backend.
 */

#include <cmath>
#include <string>

#include "numathap/backend/differentiate/Differentiator.hpp"
#include "numathap/backend/evaluate.hpp"
#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"
#include "numathap/math/prepare.hpp"

#include "numathap/numathap.hpp"
#include "test_framework.hpp"


using namespace numathap;

TEST(Differentiator, PowerConstantExponent) {
    auto ast = math::prepare("x^2");

    backend::differentiate::Differentiator differentiator;
    auto derivative = differentiator.differentiate(ast, "x");

    core::Context context;
    context.setValue("x", "3");

    auto result = backend::evaluate(derivative, context);

    EXPECT_EQ(result, core::Value(6.0));
}

TEST(Differentiator, PowerHigherConstantExponent) {
    auto ast = math::prepare("x^3");

    backend::differentiate::Differentiator differentiator;
    auto derivative = differentiator.differentiate(ast, "x");

    core::Context context;
    context.setValue("x", "2");

    auto result = backend::evaluate(derivative, context);

    EXPECT_EQ(result, core::Value(12.0));
}

TEST(Differentiator, PowerVariableExponent) {
    auto ast = math::prepare("x^x");

    backend::differentiate::Differentiator differentiator;
    auto derivative = differentiator.differentiate(ast, "x");

    core::Context context;
    context.setValue("x", "2");

    auto result = backend::evaluate(derivative, context);

    //
    // d/dx(x^x) = x^x * (log(x) + 1)
    //
    auto expectedAst = math::prepare("x^x * (log(x) + 1)");
    auto expected = backend::evaluate(expectedAst, context);

    EXPECT_EQ(result, expected);
}

TEST(Differentiator, ChainRule) {
    auto ast = math::prepare("sin(x^2)");

    backend::differentiate::Differentiator differentiator;
    auto derivative = differentiator.differentiate(ast, "x");

    core::Context context;
    context.setValue("x", "2");

    auto result = backend::evaluate(derivative, context);

    //
    // d/dx(sin(x^2)) = cos(x^2) * 2*x
    //
    auto expectedAst = math::prepare("cos(x^2) * 2*x");
    auto expected = backend::evaluate(expectedAst, context);

    EXPECT_EQ(result, expected);
}

TEST(Differentiator, ProductRule) {
    auto ast = math::prepare("x*sin(x)");

    backend::differentiate::Differentiator differentiator;
    auto derivative = differentiator.differentiate(ast, "x");

    core::Context context;
    context.setValue("x", "1");

    auto result = backend::evaluate(derivative, context);

    //
    // d/dx(x*sin(x)) = sin(x) + x*cos(x)
    //
    auto expectedAst = math::prepare("sin(x) + x*cos(x)");
    auto expected = backend::evaluate(expectedAst, context);

    EXPECT_EQ(result, expected);
}

TEST(Differentiator, QuotientRule) {
    auto ast = math::prepare("1/x");

    backend::differentiate::Differentiator differentiator;
    auto derivative = differentiator.differentiate(ast, "x");

    core::Context context;
    context.setValue("x", "2");

    auto result = backend::evaluate(derivative, context);

    //
    // d/dx(1/x) = -1/x^2
    //
    auto expectedAst = math::prepare("-1/x^2");
    auto expected = backend::evaluate(expectedAst, context);

    EXPECT_EQ(result, expected);
}

TEST(Differentiator, ConstantWithRespectToVariable) {
    auto ast = math::prepare("y");

    backend::differentiate::Differentiator differentiator;
    auto derivative = differentiator.differentiate(ast, "x");

    core::Context context;
    context.setValue("y", "7");

    auto result = backend::evaluate(derivative, context);

    EXPECT_EQ(result, core::Value(0.0));
}

TEST(Differentiator, Number) {
    auto ast = math::prepare("42");

    backend::differentiate::Differentiator differentiator;
    auto derivative = differentiator.differentiate(ast, "x");

    core::Context context;

    auto result = backend::evaluate(derivative, context);

    EXPECT_EQ(result, core::Value(0.0));
}

TEST(Differentiator, EmptyVariableThrows) {
    auto ast = math::prepare("x^2");

    backend::differentiate::Differentiator differentiator;

    EXPECT_THROWS(differentiator.differentiate(ast, ""),
                  std::invalid_argument);
}

int main() { return testfw::runAll(); }