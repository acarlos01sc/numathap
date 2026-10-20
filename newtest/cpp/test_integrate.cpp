#include <gtest/gtest.h>

#include <cmath>

#include "numathap/backend/integrate/GaussKronrod15.hpp"
#include "numathap/numathap.hpp"

namespace {

using namespace numathap;

constexpr double kExpectedResult = 22.1406926327793;
constexpr double kTolerance = 1e-8;

TEST(NumathapIntegrate, AdaptiveSimpson) {
    const auto expression = prepare("e^x");

    Context context;
    context.setInterval("x", "0", "pi");

    const auto environment =
        configure(backend::integrate::AdaptiveSimpsonConfig{});

    const Value result = integrate(expression, "x", context, environment);

    ASSERT_TRUE(result.isReal());

    EXPECT_NEAR(result.real().value(), kExpectedResult, kTolerance);
}

TEST(NumathapIntegrate, GaussKronrod15) {
    const auto expression = prepare("e^x");

    Context context;
    context.setInterval("x", "0", "pi");

    const auto environment =
        configure(backend::integrate::GaussKronrod15Config{});

    const Value result = integrate(expression, "x", context, environment);

    ASSERT_TRUE(result.isReal());

    EXPECT_NEAR(result.real().value(), kExpectedResult, kTolerance);
}

TEST(NumathapIntegrate, AdaptiveSimpsonGaussian) {
    const auto expression = prepare("e^(-x^2)");

    Context context;
    context.setInterval("x", "-inf", "inf");

    const auto environment =
        configure(backend::integrate::AdaptiveSimpsonConfig{});

    const Value result = integrate(expression, "x", context, environment);

    ASSERT_TRUE(result.isReal());

    EXPECT_NEAR(result.real().value(), 1.772453850905516, kTolerance);
}

TEST(NumathapIntegrate, GaussKronrod15Gaussian) {
    const auto expression = prepare("e^(-x^2)");

    Context context;
    context.setInterval("x", "-inf", "inf");

    const auto environment =
        configure(backend::integrate::GaussKronrod15Config{});

    const Value result = integrate(expression, "x", context, environment);

    ASSERT_TRUE(result.isReal());

    EXPECT_NEAR(result.real().value(), 1.772453850905516, kTolerance);
}

TEST(NumathapIntegrate, GaussKronrod15SincSingularity) {
    const auto expression = prepare("sin(x)/x");

    Context context;
    context.setInterval("x", "-1", "1");

    const auto environment =
        configure(backend::integrate::GaussKronrod15Config{});

    const Value result = integrate(expression, "x", context, environment);

    ASSERT_TRUE(result.isReal());

    constexpr double kExpectedResult = 1.892166140734366;
    constexpr double kTolerance = 1e-10;

    EXPECT_NEAR(result.real().value(), kExpectedResult, kTolerance);
}

TEST(NumathapIntegrate, AdaptiveSimpsonWithParameter) {
    const auto expression = prepare("x^2 + y");

    Context context;
    context.setInterval("x", "0", "1");
    context.setValue("y", "2");

    const auto environment =
        configure(backend::integrate::AdaptiveSimpsonConfig{});

    const Value result = integrate(expression, "x", context, environment);

    ASSERT_TRUE(result.isReal());

    EXPECT_NEAR(result.real().value(), 7.0 / 3.0, kTolerance);
}

TEST(NumathapIntegrate, GaussKronrod15WithParameter) {
    const auto expression = prepare("x^2 + y");

    Context context;
    context.setInterval("x", "0", "1");
    context.setValue("y", "2");

    const auto environment =
        configure(backend::integrate::GaussKronrod15Config{});

    const Value result = integrate(expression, "x", context, environment);

    ASSERT_TRUE(result.isReal());

    EXPECT_NEAR(result.real().value(), 7.0 / 3.0, kTolerance);
}


}  // namespace