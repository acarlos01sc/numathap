#include <gtest/gtest.h>

#include "numathap/numathap.hpp"

using namespace numathap;

TEST(NumathapPublicApi, LimitFiniteByDirectSubstitution) {
    const auto expression = math::prepare("x^2 + 1");

    const auto result = backend::limit::limit(expression, "x", "2");

    EXPECT_EQ(expression.expression(), "x^2 + 1");

    ASSERT_EQ(result.status, backend::limit::LimitStatus::Finite);
    ASSERT_TRUE(result.value.isReal());
    EXPECT_DOUBLE_EQ(result.value.real().value(), 5.0);
}

TEST(NumathapPublicApi, LimitIndeterminateFormSimplifiedBeforeEvaluation) {
    const auto expression = math::prepare("(x - 1) / (x - 1)");

    const auto result = backend::limit::limit(expression, "x", "1");

    ASSERT_EQ(result.status, backend::limit::LimitStatus::Finite);
    ASSERT_TRUE(result.value.isReal());
    EXPECT_DOUBLE_EQ(result.value.real().value(), 1.0);
}

TEST(NumathapPublicApi, LimitCommonFactorCancellation) {
    const auto expression = math::prepare("x * (x + 1) / (x * (x + 2))");

    const auto result = backend::limit::limit(expression, "x", "0");

    ASSERT_EQ(result.status, backend::limit::LimitStatus::Finite);
    ASSERT_TRUE(result.value.isReal());
    EXPECT_NEAR(result.value.real().value(), 0.5, 1e-12);
}

TEST(NumathapPublicApi, LimitCompositeCommonFactorCancellation) {
    const auto expression =
        math::prepare("((x + 1) * (x - 2)) / ((x + 3) * (x - 2))");

    const auto result = backend::limit::limit(expression, "x", "2");

    ASSERT_EQ(result.status, backend::limit::LimitStatus::Finite);
    ASSERT_TRUE(result.value.isReal());
    EXPECT_NEAR(result.value.real().value(), 0.6, 1e-12);
}

TEST(NumathapPublicApi, LimitUnsupportedIndeterminateForm) {
    const auto expression = math::prepare("sin(x) / x");

    const auto result = backend::limit::limit(expression, "x", "0");

    EXPECT_EQ(result.status, backend::limit::LimitStatus::Failed);
}

TEST(NumathapPublicApi, LimitReciprocalFromRightAtZero) {
    const auto expression = math::prepare("1 / x");

    const auto result = backend::limit::limit(expression, "x", "0+");

    EXPECT_EQ(result.status, backend::limit::LimitStatus::PositiveInfinity);
}

TEST(NumathapPublicApi, LimitReciprocalFromLeftAtZero) {
    const auto expression = math::prepare("1 / x");

    const auto result = backend::limit::limit(expression, "x", "0-");

    EXPECT_EQ(result.status, backend::limit::LimitStatus::NegativeInfinity);
}

TEST(NumathapPublicApi, LimitReciprocalBilateralAtZero) {
    const auto expression = math::prepare("1 / x");

    const auto result = backend::limit::limit(expression, "x", "0");

    EXPECT_EQ(result.status, backend::limit::LimitStatus::DoesNotExist);
}

TEST(NumathapPublicApi, LimitInverseSquareBilateralAtZero) {
    const auto expression = math::prepare("x^(-2)");

    const auto result = backend::limit::limit(expression, "x", "0");

    EXPECT_EQ(result.status, backend::limit::LimitStatus::PositiveInfinity);
}

TEST(NumathapPublicApi, LimitInverseCubeBilateralAtZero) {
    const auto expression = math::prepare("x^(-3)");

    const auto result = backend::limit::limit(expression, "x", "0");

    EXPECT_EQ(result.status, backend::limit::LimitStatus::DoesNotExist);
}

TEST(NumathapPublicApi, LimitShiftedReciprocalFromRight) {
    const auto expression = math::prepare("(x - 2)^(-1)");

    const auto result = backend::limit::limit(expression, "x", "2+");

    EXPECT_EQ(result.status, backend::limit::LimitStatus::PositiveInfinity);
}

TEST(NumathapPublicApi, LimitScaledInverseSquareWithFiniteTerm) {
    const auto expression = math::prepare("2 * (3*x + 6)^(-2) + 7");

    const auto result = backend::limit::limit(expression, "x", "-2");

    EXPECT_EQ(result.status, backend::limit::LimitStatus::PositiveInfinity);
}

TEST(NumathapPublicApi, LimitScaledReciprocalFromRight) {
    const auto expression = math::prepare("1/(3*x+6)");

    const auto result = backend::limit::limit(expression, "x", "-2+");

    EXPECT_EQ(result.status, backend::limit::LimitStatus::PositiveInfinity);
}

// Limits at infinity.

TEST(NumathapPublicApi, LimitReciprocalAffineAtPlusInfinite) {
    const auto expression = math::prepare("1/(3*x+6)");

    const auto result = backend::limit::limit(expression, "x", "+inf");

    ASSERT_EQ(result.status, backend::limit::LimitStatus::Finite);
    ASSERT_TRUE(result.value.isReal());
    EXPECT_DOUBLE_EQ(result.value.real().value(), 0.0);
}

TEST(NumathapPublicApi, LimitReciprocalAffineAtMinusInfinite) {
    const auto expression = math::prepare("1/(3*x+6)");

    const auto result = backend::limit::limit(expression, "x", "-inf");

    ASSERT_EQ(result.status, backend::limit::LimitStatus::Finite);
    ASSERT_TRUE(result.value.isReal());
    EXPECT_DOUBLE_EQ(result.value.real().value(), 0.0);
}

TEST(NumathapPublicApi, LimitVariableAtPlusInfinite) {
    const auto expression = math::prepare("x");

    const auto result = backend::limit::limit(expression, "x", "+inf");

    EXPECT_EQ(result.status, backend::limit::LimitStatus::PositiveInfinity);
}

TEST(NumathapPublicApi, LimitVariableAtMinusInfinite) {
    const auto expression = math::prepare("x");

    const auto result = backend::limit::limit(expression, "x", "-inf");

    EXPECT_EQ(result.status, backend::limit::LimitStatus::NegativeInfinity);
}

TEST(NumathapPublicApi, LimitEvenPowerAtMinusInfinite) {
    const auto expression = math::prepare("x^2");

    const auto result = backend::limit::limit(expression, "x", "-inf");

    EXPECT_EQ(result.status, backend::limit::LimitStatus::PositiveInfinity);
}

// The current strategies must not claim to resolve these indeterminate forms.

TEST(NumathapPublicApi, LimitRationalFunctionAtPlusInfinite) {
    const auto expression = math::prepare("x/(x+1)");

    const auto result = backend::limit::limit(expression, "x", "+inf");

    EXPECT_EQ(result.status, backend::limit::LimitStatus::Failed);
}

TEST(NumathapPublicApi, LimitPolynomialRatioAtPlusInfinite) {
    const auto expression = math::prepare("(2*x^2+3)/(x^2-1)");

    const auto result = backend::limit::limit(expression, "x", "+inf");

    EXPECT_EQ(result.status, backend::limit::LimitStatus::Failed);
}