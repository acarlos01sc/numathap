#include <gtest/gtest.h>

#include <numathap/numathap.hpp>

TEST(NumathapPublicApi, PrepareExpression)
{
    EXPECT_NO_THROW(
        {
            auto ast = numathap::prepare("x^2");
            (void)ast;
        }
    );
}