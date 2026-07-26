/**
 * @file TEST_integrate_cfg.cpp
 * @brief Tests integration algorithm configuration through Configurator.
 */

#include "test_framework.hpp"

#include <variant>

#include "numathap/backend/integrate/AdaptiveSimpson.hpp"
#include "numathap/backend/integrate/Algorithm.hpp"
#include "numathap/config/MathEnvironment.hpp"
#include "numathap/config/configure.hpp"

namespace {

using numathap::backend::integrate::AdaptiveSimpsonConfig;
using numathap::backend::integrate::Algorithm;
using numathap::config::configure;

TEST(ConfiguratorIntegrationTest, UsesAdaptiveSimpsonDefaults) {
    const auto environment = configure();

    EXPECT_EQ(
        environment.integrationAlgorithm(),
        Algorithm::AdaptiveSimpson);

    const auto& algorithmConfig =
        environment.integrationAlgorithmConfig();

    EXPECT_TRUE(
        std::holds_alternative<AdaptiveSimpsonConfig>(
            algorithmConfig));

    const auto& config =
        std::get<AdaptiveSimpsonConfig>(algorithmConfig);

    EXPECT_EQ(config.tolerance, numathap::core::Value(1e-10));
    EXPECT_EQ(config.maxDepth, 20);
}

TEST(ConfiguratorIntegrationTest, AcceptsAdaptiveSimpsonConfiguration) {
    const AdaptiveSimpsonConfig expected{
        .tolerance = numathap::core::Value(1e-12),
        .maxDepth = 30
    };

    const auto environment = configure(expected);

    EXPECT_EQ(
        environment.integrationAlgorithm(),
        Algorithm::AdaptiveSimpson);

    const auto& algorithmConfig =
        environment.integrationAlgorithmConfig();

    EXPECT_TRUE(
        std::holds_alternative<AdaptiveSimpsonConfig>(
            algorithmConfig));

    const auto& config =
        std::get<AdaptiveSimpsonConfig>(algorithmConfig);

    EXPECT_EQ(config.tolerance, expected.tolerance);
    EXPECT_EQ(config.maxDepth, expected.maxDepth);
}

}  // namespace

int main() {
    return testfw::runAll();
}