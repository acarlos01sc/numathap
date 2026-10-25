#include <gtest/gtest.h>

#include <numathap/numathap.hpp>



namespace {

void expectFiniteLimit(
    const std::string& expression,
    const std::string& point,
    double expected
)
{
    const auto prepared = numathap::prepare(expression);
    const auto result = numathap::limit(prepared, "x", point);

    ASSERT_EQ(result.status, numathap::LimitStatus::Finite);

    const auto value = result.value.real();
    ASSERT_TRUE(value.has_value());

    EXPECT_NEAR(*value, expected, 1e-8);
}

}  // namespace

TEST(Limit, PolynomialAtRegularPoint)
{
    expectFiniteLimit("x^2 + 3*x", "2", 10.0);
}

TEST(Limit, RemovableSingularity)
{
    expectFiniteLimit("(x^2 - 1)/(x - 1)", "1", 2.0);
}

TEST(Limit, SincAtOrigin)
{
    expectFiniteLimit("sin(x)/x", "0", 1.0);
}

TEST(Limit, PositiveInfinityFromRight)
{
    const auto expression = numathap::prepare("1/x");
    const auto result = numathap::limit(expression, "x", "0+");

    EXPECT_EQ(
        result.status,
        numathap::LimitStatus::PositiveInfinity
    );
}

TEST(Limit, NegativeInfinityFromLeft)
{
    const auto expression = numathap::prepare("1/x");
    const auto result = numathap::limit(expression, "x", "0-");

    EXPECT_EQ(
        result.status,
        numathap::LimitStatus::NegativeInfinity
    );
}

TEST(Limit, BilateralLimitDoesNotExist)
{
    const auto expression = numathap::prepare("1/x");
    const auto result = numathap::limit(expression, "x", "0");

    EXPECT_EQ(
        result.status,
        numathap::LimitStatus::DoesNotExist
    );
}

TEST(Limit, BilateralLimitCanBePositiveInfinity)
{
    const auto expression = numathap::prepare("1/x^2");
    const auto result = numathap::limit(expression, "x", "0");

    EXPECT_EQ(
        result.status,
        numathap::LimitStatus::PositiveInfinity
    );
}
