#include <gtest/gtest.h>

#include "numathap/numathap.hpp"

using namespace numathap;

TEST(NumathapPublicApi, LimitPreservesOriginalExpression) {
    const auto expression = math::prepare("x + 0");
    const auto result = backend::limit::limit(expression, "x", "2");

    EXPECT_EQ(expression.expression(), "x + 0");
    ASSERT_EQ(result.status, backend::limit::LimitStatus::Finite);
    ASSERT_TRUE(result.value.isReal());
    EXPECT_DOUBLE_EQ(result.value.real().value(), 2.0);
}

TEST(NumathapPublicApi, LimitPreservesOriginalExpressionBehavior) {
    const auto expression = numathap::math::prepare("x + 0");
    const auto result = numathap::backend::limit::limit(expression, "x", "2");

    numathap::core::Context context;
    context.setValue("x", "2");
    const auto value = numathap::backend::evaluate(expression, context);

    ASSERT_EQ(result.status, numathap::backend::limit::LimitStatus::Finite);
    ASSERT_TRUE(result.value.isReal());
    EXPECT_DOUBLE_EQ(result.value.real().value(), 2.0);

    ASSERT_TRUE(value.isReal());
    EXPECT_DOUBLE_EQ(value.real().value(), 2.0);
    EXPECT_EQ(expression.expression(), "x + 0");
}

TEST(NumathapPublicApi, LimitFiniteByDirectSubstitution) {
    const auto expression = numathap::math::prepare("x^2 + 1");

    const auto result = numathap::backend::limit::limit(expression, "x", "2");

    ASSERT_EQ(result.status, numathap::backend::limit::LimitStatus::Finite);

    ASSERT_TRUE(result.value.isReal());
    EXPECT_NEAR(result.value.real().value(), 5.0, 1e-12);
}
