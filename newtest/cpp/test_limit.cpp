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
    auto expression = math::prepare("x * (x + 1) / (x * (x + 2))");

    const auto result = backend::limit::limit(expression, "x", "0");

    ASSERT_EQ(result.status, backend::limit::LimitStatus::Finite);
    EXPECT_NEAR(result.value.real().value(), 0.5, 1e-12);
}

TEST(NumathapPublicApi, LimitCompositeCommonFactorCancellation) {
    auto expression =
        math::prepare("((x + 1) * (x - 2)) / ((x + 3) * (x - 2))");

    const auto result = backend::limit::limit(expression, "x", "2");

    ASSERT_EQ(result.status, backend::limit::LimitStatus::Finite);
    EXPECT_NEAR(result.value.real().value(), 0.6, 1e-12);
}

TEST(NumathapPublicApi, LimitDirectSubstitutionBilateralAtZero) {
    const auto expression = math::prepare("x");

    const auto result = backend::limit::limit(expression, "x", "0");

    ASSERT_EQ(result.status, backend::limit::LimitStatus::Finite);
    EXPECT_DOUBLE_EQ(result.value.real().value(), 0.0);
}

TEST(NumathapPublicApi, LimitDirectSubstitutionRightAtZero) {
    const auto expression = math::prepare("x");

    const auto result = backend::limit::limit(expression, "x", "0+");

    ASSERT_EQ(result.status, backend::limit::LimitStatus::Finite);
    EXPECT_DOUBLE_EQ(result.value.real().value(), 0.0);
}

TEST(NumathapPublicApi, LimitDirectSubstitutionLeftAtZero) {
    const auto expression = math::prepare("x");

    const auto result = backend::limit::limit(expression, "x", "0-");

    ASSERT_EQ(result.status, backend::limit::LimitStatus::Finite);
    EXPECT_DOUBLE_EQ(result.value.real().value(), 0.0);
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

TEST(NumathapPublicApi, LimitInverseSquareFromRightAtZero) {
    const auto expression = math::prepare("x^(-2)");

    const auto result = backend::limit::limit(expression, "x", "0+");

    EXPECT_EQ(result.status, backend::limit::LimitStatus::PositiveInfinity);
}

TEST(NumathapPublicApi, LimitInverseSquareFromLeftAtZero) {
    const auto expression = math::prepare("x^(-2)");

    const auto result = backend::limit::limit(expression, "x", "0-");

    EXPECT_EQ(result.status, backend::limit::LimitStatus::PositiveInfinity);
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

TEST(NumathapPublicApi, LimitShiftedReciprocalFromLeft) {
    const auto expression = math::prepare("(x - 2)^(-1)");

    const auto result = backend::limit::limit(expression, "x", "2-");

    EXPECT_EQ(result.status, backend::limit::LimitStatus::NegativeInfinity);
}

TEST(NumathapPublicApi, LimitScaledInverseSquareWithFiniteTerm) {
    const auto expression = math::prepare("2 * (3*x + 6)^(-2) + 7");

    const auto result = backend::limit::limit(expression, "x", "-2");

    EXPECT_EQ(result.status, backend::limit::LimitStatus::PositiveInfinity);
}

TEST(NumathapPublicApi, LimitScaledReciprocalFromRight) {
    auto expression = math::prepare("1/(3*x+6)");

    const auto result = backend::limit::limit(expression, "x", "-2+");

    EXPECT_EQ(result.status, backend::limit::LimitStatus::PositiveInfinity);
}