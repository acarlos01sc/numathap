#include <gtest/gtest.h>

#include <numathap/numathap.hpp>

#include <numathap/config/Capability.hpp>
#include <numathap/math/MathNode.hpp>

using numathap::config::Capability;
using numathap::math::BinaryNode;
using numathap::math::BinaryOp;
using numathap::math::NumberNode;
using numathap::math::SymbolNode;

// ============================================================
// ConstantFolder
// ============================================================

TEST(MathEnvironment, ConstantFolderEnabled)
{
    auto environment = numathap::configure(
        Capability::ConstantFolder
    );

    ASSERT_TRUE(
        environment.hasCapability(Capability::ConstantFolder)
    );

    auto ast = numathap::prepare("2 + 3", environment);

    ASSERT_NE(ast.root(), nullptr);

    const auto* result =
        dynamic_cast<const NumberNode*>(ast.root());

    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->value, "5");
}

// ============================================================
// Simplify
// ============================================================

TEST(MathEnvironment, SimplifyEnabled)
{
    auto environment = numathap::configure(
        Capability::Simplify
    );

    ASSERT_TRUE(
        environment.hasCapability(Capability::Simplify)
    );

    auto ast = numathap::prepare("x + 0", environment);

    ASSERT_NE(ast.root(), nullptr);

    const auto* result =
        dynamic_cast<const SymbolNode*>(ast.root());

    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->name, "x");
}

// ============================================================
// ConstantFolder and Simplify
// ============================================================

TEST(MathEnvironment, PreparedAstPreservesEnvironment)
{
    auto environment = numathap::configure(
        Capability::ConstantFolder,
        Capability::Simplify
    );

    auto ast = numathap::prepare("x + (2 + 3) + 0", environment);

    // Verifica se o ambiente foi preservado.
    EXPECT_TRUE(
        ast.environment().hasCapability(Capability::ConstantFolder)
    );

    EXPECT_TRUE(
        ast.environment().hasCapability(Capability::Simplify)
    );

    // Verifica o resultado da preparação.
    ASSERT_NE(ast.root(), nullptr);

    const auto* result =
        dynamic_cast<const BinaryNode*>(ast.root());

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->op, BinaryOp::Add);

    const auto* left =
        dynamic_cast<const SymbolNode*>(result->left.get());

    ASSERT_NE(left, nullptr);
    EXPECT_EQ(left->name, "x");

    const auto* right =
        dynamic_cast<const NumberNode*>(result->right.get());

    ASSERT_NE(right, nullptr);
    EXPECT_EQ(right->value, "5");
}

