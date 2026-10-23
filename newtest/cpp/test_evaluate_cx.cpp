#include <gtest/gtest.h>

#include <numbers>
#include <memory>
#include <string>

#include "numathap/numathap.hpp"

namespace {

using namespace numathap;

constexpr auto kTolerance = 1e-8;

MathEnvironment makeComplexEnvironment() {
    return MathEnvironment(std::make_unique<ComplexDoubleAdapter>());
}

void expectComplexNear(const Value& result, auto expectedReal,
                       auto expectedImaginary) {
    ASSERT_TRUE(result.isComplex())
        << "Expected a complex result, got: " << result;

    const auto& value = result.complex();

    EXPECT_NEAR(value.real().value(), expectedReal, kTolerance);

    EXPECT_NEAR(value.imaginary().value(), expectedImaginary, kTolerance);
}

Value evaluateComplexExpression(const char* expression, const char* variable,
                                const char* variableValue) {
    auto environment = makeComplexEnvironment();
    auto prepared = prepare(expression, environment);

    Context context;
    context.setValue(variable, variableValue);

    return evaluate_cx(prepared, context);
}

TEST(NumathapEvaluateComplex, SineOfImaginaryArgument) {
    auto environment = makeComplexEnvironment();
    auto expression = prepare("sin(z)", environment);

    Context context;
    context.setValue("z", "i*pi");

    const auto result = evaluate_cx(expression, context);

    expectComplexNear(result, 0.0, 11.548739357257748);
}

TEST(NumathapEvaluateComplex, ComplexPower) {
    const auto result = evaluateComplexExpression("z^4", "z", "1+i");

    expectComplexNear(result, -4.0, 0.0);
}

TEST(NumathapEvaluateComplex, Addition) {
    const auto result = evaluateComplexExpression("z+2", "z", "3+4*i");

    expectComplexNear(result, 5.0, 4.0);
}

TEST(NumathapEvaluateComplex, Multiplication) {
    const auto result = evaluateComplexExpression("z*z", "z", "1+i");

    expectComplexNear(result, 0.0, 2.0);
}

TEST(NumathapEvaluateComplex, Division) {
    const auto result = evaluateComplexExpression("1/z", "z", "1+i");

    expectComplexNear(result, 0.5, -0.5);
}

TEST(NumathapEvaluateComplex, ExponentialOfImaginaryArgument) {
    const auto result = evaluateComplexExpression("exp(z)", "z", "i*pi");

    expectComplexNear(result, -1.0, 0.0);
}

TEST(NumathapEvaluateComplex, EulerIdentity) {
    const auto result = evaluateComplexExpression("exp(i*pi)+1", "z", "0");

    expectComplexNear(result, 0.0, 0.0);
}

TEST(NumathapEvaluateComplex, MultipleComplexVariables) {
    auto environment = makeComplexEnvironment();
    auto expression = prepare("z*w", environment);

    Context context;
    context.setValue("z", "1+i");
    context.setValue("w", "2-i");

    const auto result = evaluate_cx(expression, context);

    expectComplexNear(result, 3.0, 1.0);
}

TEST(NumathapEvaluateComplex, ComplexLogarithmPrincipalBranch) {
    const auto result = evaluateComplexExpression("log(z)", "z", "i");

    expectComplexNear(result, 0.0, std::numbers::pi / 2.0);
}

class ComplexLogarithmBranchTest : public ::testing::TestWithParam<int> {};

TEST_P(ComplexLogarithmBranchTest, SelectsRequestedBranch) {
    const int branch = GetParam();

    const auto result = evaluateComplexExpression(
        ("log(z, " + std::to_string(branch) + ")").c_str(), "z", "i");

    const auto expectedImaginary =
        std::numbers::pi / 2.0 + 2.0 * std::numbers::pi * branch;

    expectComplexNear(result, 0.0, expectedImaginary);
}

INSTANTIATE_TEST_SUITE_P(Branches, ComplexLogarithmBranchTest,
                         ::testing::Values(-1, 0, 1));

TEST(NumathapEvaluateComplex, ComplexSquareRootPrincipalBranch) {
    const auto result = evaluateComplexExpression("sqrt(z)", "z", "-1");

    expectComplexNear(result, 0.0, 1.0);
}

class ComplexSquareRootBranchTest : public ::testing::TestWithParam<int> {};

TEST_P(ComplexSquareRootBranchTest, SelectsRequestedBranch) {
    const int branch = GetParam();

    const auto result = evaluateComplexExpression(
        ("sqrt(z, " + std::to_string(branch) + ")").c_str(), "z", "-1");

    const auto expectedImaginary = (branch % 2 == 0) ? 1.0 : -1.0;

    expectComplexNear(result, 0.0, expectedImaginary);
}

INSTANTIATE_TEST_SUITE_P(Branches, ComplexSquareRootBranchTest,
                         ::testing::Values(-2, -1, 0, 1, 2));

TEST(NumathapEvaluateComplex, ComplexPowerPrincipalBranch) {
    auto environment = makeComplexEnvironment();
    auto expression = prepare("pow(z, w)", environment);

    Context context;
    context.setValue("z", "-1");
    context.setValue("w", "0.5");

    const auto result = evaluate_cx(expression, context);

    expectComplexNear(result, 0.0, 1.0);
}

class ComplexPowerBranchTest : public ::testing::TestWithParam<int> {};

TEST_P(ComplexPowerBranchTest, SelectsRequestedBranch) {
    const int branch = GetParam();

    const auto expressionText = "pow(z, w, " + std::to_string(branch) + ")";

    auto environment = makeComplexEnvironment();
    auto expression = prepare(expressionText, environment);

    Context context;
    context.setValue("z", "-1");
    context.setValue("w", "0.5");

    const auto result = evaluate_cx(expression, context);

    const auto expectedImaginary = (branch % 2 == 0) ? 1.0 : -1.0;

    expectComplexNear(result, 0.0, expectedImaginary);
}

INSTANTIATE_TEST_SUITE_P(Branches, ComplexPowerBranchTest,
                         ::testing::Values(-2, -1, 0, 1, 2));

}  // namespace