#include <gtest/gtest.h>

#include <numathap/numathap.hpp>
#include <numathap/math/PreparedAst.hpp>
#include <numathap/math/MathNode.hpp>

using namespace numathap::math;


// ============================================================
// Auxiliar para verificar o tipo de um nó.
// ============================================================

template <typename T>
const T* as_node(const MathNode* node)
{
    return dynamic_cast<const T*>(node);
}


// ============================================================
// Testes básicos
// ============================================================

TEST(NumathapPublicApi, PrepareException)
{
    EXPECT_NO_THROW(
        {
            auto ast = numathap::prepare("x^2");
            (void)ast;
        }
    );
}


TEST(NumathapPublicApi, PrepareExpression)
{
    auto ast = numathap::prepare("x^2");

    ASSERT_NE(ast.root(), nullptr);

    const auto* power = as_node<BinaryNode>(ast.root());
    ASSERT_NE(power, nullptr);

    EXPECT_EQ(power->op, BinaryOp::Power);

    ASSERT_NE(power->left, nullptr);
    ASSERT_NE(power->right, nullptr);

    const auto* variable = as_node<SymbolNode>(power->left.get());
    ASSERT_NE(variable, nullptr);
    EXPECT_EQ(variable->name, "x");

    const auto* exponent = as_node<NumberNode>(power->right.get());
    ASSERT_NE(exponent, nullptr);
    EXPECT_EQ(exponent->value, "2");
}


// ============================================================
// Teste 1: sin(x^2)
// ============================================================

TEST(NumathapPublicApi, PrepareNestedPowerFunction)
{
    auto ast = numathap::prepare("sin(x^2)");

    ASSERT_NE(ast.root(), nullptr);

    // Raiz: sin(...)
    const auto* sin_node = as_node<FunctionNode>(ast.root());

    ASSERT_NE(sin_node, nullptr);
    EXPECT_EQ(sin_node->name, "sin");

    ASSERT_EQ(sin_node->arguments.size(), 1);

    // Argumento de sin: x^2
    const auto* power =
        as_node<BinaryNode>(sin_node->arguments[0].get());

    ASSERT_NE(power, nullptr);
    EXPECT_EQ(power->op, BinaryOp::Power);

    ASSERT_NE(power->left, nullptr);
    ASSERT_NE(power->right, nullptr);

    // Base: x
    const auto* variable = as_node<SymbolNode>(power->left.get());

    ASSERT_NE(variable, nullptr);
    EXPECT_EQ(variable->name, "x");

    // Expoente: 2
    const auto* exponent = as_node<NumberNode>(power->right.get());

    ASSERT_NE(exponent, nullptr);
    EXPECT_EQ(exponent->value, "2");
}


// ============================================================
// Teste 2: sqrt(x)/tan(log(x))
// ============================================================

TEST(NumathapPublicApi, PrepareNestedFunctionsAndDivision)
{
    auto ast = numathap::prepare("sqrt(x)/tan(log(x))");

    ASSERT_NE(ast.root(), nullptr);

    // Raiz: divisão
    const auto* division = as_node<BinaryNode>(ast.root());

    ASSERT_NE(division, nullptr);
    EXPECT_EQ(division->op, BinaryOp::Divide);

    ASSERT_NE(division->left, nullptr);
    ASSERT_NE(division->right, nullptr);

    // Numerador: sqrt(x)
    const auto* sqrt_node = as_node<FunctionNode>(division->left.get());

    ASSERT_NE(sqrt_node, nullptr);
    EXPECT_EQ(sqrt_node->name, "sqrt");

    ASSERT_EQ(sqrt_node->arguments.size(), 1);

    const auto* numerator_variable =
        as_node<SymbolNode>(sqrt_node->arguments[0].get());

    ASSERT_NE(numerator_variable, nullptr);
    EXPECT_EQ(numerator_variable->name, "x");

    // Denominador: tan(log(x))
    const auto* tan_node = as_node<FunctionNode>(division->right.get());

    ASSERT_NE(tan_node, nullptr);
    EXPECT_EQ(tan_node->name, "tan");

    ASSERT_EQ(tan_node->arguments.size(), 1);

    // Argumento de tan: log(x)
    const auto* log_node =
        as_node<FunctionNode>(tan_node->arguments[0].get());

    ASSERT_NE(log_node, nullptr);
    EXPECT_EQ(log_node->name, "log");

    ASSERT_EQ(log_node->arguments.size(), 1);

    // Argumento de log: x
    const auto* denominator_variable =
        as_node<SymbolNode>(log_node->arguments[0].get());

    ASSERT_NE(denominator_variable, nullptr);
    EXPECT_EQ(denominator_variable->name, "x");
}


TEST(NumathapPublicApi, PrepareComplexExpression)
{
    auto ast = numathap::prepare(
        "(tanh(x)+sin(log(x))+x^(3/2))/(tan(x^2)+20*pi)"
    );

    ASSERT_NE(ast.root(), nullptr);

    // ========================================================
    // Raiz: divisão
    // ========================================================

    const auto* division = as_node<BinaryNode>(ast.root());

    ASSERT_NE(division, nullptr);
    ASSERT_EQ(division->op, BinaryOp::Divide);

    ASSERT_NE(division->left, nullptr);
    ASSERT_NE(division->right, nullptr);

    // ========================================================
    // Numerador: tanh(x) + sin(log(x)) + x^(3/2)
    //
    // Estrutura esperada:
    //
    //              Add
    //             /   \
    //           Add   Power
    //          /   \   /  \
    //       tanh   sin x  Divide
    //        |      |      / \
    //        x     log    3   2
    //                |
    //                x
    // ========================================================

    const auto* numerator =
        as_node<BinaryNode>(division->left.get());

    ASSERT_NE(numerator, nullptr);
    ASSERT_EQ(numerator->op, BinaryOp::Add);

    ASSERT_NE(numerator->left, nullptr);
    ASSERT_NE(numerator->right, nullptr);

    // --------------------------------------------------------
    // Parcela: x^(3/2)
    // --------------------------------------------------------

    const auto* fractional_power =
        as_node<BinaryNode>(numerator->right.get());

    ASSERT_NE(fractional_power, nullptr);
    ASSERT_EQ(fractional_power->op, BinaryOp::Power);

    ASSERT_NE(fractional_power->left, nullptr);
    ASSERT_NE(fractional_power->right, nullptr);

    // Base: x
    const auto* power_base =
        as_node<SymbolNode>(fractional_power->left.get());

    ASSERT_NE(power_base, nullptr);
    EXPECT_EQ(power_base->name, "x");

    // Expoente: 3/2
    const auto* power_exponent =
        as_node<BinaryNode>(fractional_power->right.get());

    ASSERT_NE(power_exponent, nullptr);
    ASSERT_EQ(power_exponent->op, BinaryOp::Divide);

    ASSERT_NE(power_exponent->left, nullptr);
    ASSERT_NE(power_exponent->right, nullptr);

    const auto* exponent_numerator =
        as_node<NumberNode>(power_exponent->left.get());

    ASSERT_NE(exponent_numerator, nullptr);
    EXPECT_EQ(exponent_numerator->value, "3");

    const auto* exponent_denominator =
        as_node<NumberNode>(power_exponent->right.get());

    ASSERT_NE(exponent_denominator, nullptr);
    EXPECT_EQ(exponent_denominator->value, "2");

    // --------------------------------------------------------
    // Soma: tanh(x) + sin(log(x))
    // --------------------------------------------------------

    const auto* numerator_sum =
        as_node<BinaryNode>(numerator->left.get());

    ASSERT_NE(numerator_sum, nullptr);
    ASSERT_EQ(numerator_sum->op, BinaryOp::Add);

    ASSERT_NE(numerator_sum->left, nullptr);
    ASSERT_NE(numerator_sum->right, nullptr);

    // tanh(x)
    const auto* tanh_node =
        as_node<FunctionNode>(numerator_sum->left.get());

    ASSERT_NE(tanh_node, nullptr);
    EXPECT_EQ(tanh_node->name, "tanh");

    ASSERT_EQ(tanh_node->arguments.size(), 1);

    const auto* tanh_argument =
        as_node<SymbolNode>(tanh_node->arguments[0].get());

    ASSERT_NE(tanh_argument, nullptr);
    EXPECT_EQ(tanh_argument->name, "x");

    // sin(log(x))
    const auto* sin_node =
        as_node<FunctionNode>(numerator_sum->right.get());

    ASSERT_NE(sin_node, nullptr);
    EXPECT_EQ(sin_node->name, "sin");

    ASSERT_EQ(sin_node->arguments.size(), 1);

    // log(x)
    const auto* log_node =
        as_node<FunctionNode>(sin_node->arguments[0].get());

    ASSERT_NE(log_node, nullptr);
    EXPECT_EQ(log_node->name, "log");

    ASSERT_EQ(log_node->arguments.size(), 1);

    const auto* log_argument =
        as_node<SymbolNode>(log_node->arguments[0].get());

    ASSERT_NE(log_argument, nullptr);
    EXPECT_EQ(log_argument->name, "x");

    // ========================================================
    // Denominador: tan(x^2) + 20*pi
    // ========================================================

    const auto* denominator =
        as_node<BinaryNode>(division->right.get());

    ASSERT_NE(denominator, nullptr);
    ASSERT_EQ(denominator->op, BinaryOp::Add);

    ASSERT_NE(denominator->left, nullptr);
    ASSERT_NE(denominator->right, nullptr);

    // --------------------------------------------------------
    // Parcela: tan(x^2)
    // --------------------------------------------------------

    const auto* tan_node =
        as_node<FunctionNode>(denominator->left.get());

    ASSERT_NE(tan_node, nullptr);
    EXPECT_EQ(tan_node->name, "tan");

    ASSERT_EQ(tan_node->arguments.size(), 1);

    const auto* denominator_power =
        as_node<BinaryNode>(tan_node->arguments[0].get());

    ASSERT_NE(denominator_power, nullptr);
    ASSERT_EQ(denominator_power->op, BinaryOp::Power);

    ASSERT_NE(denominator_power->left, nullptr);
    ASSERT_NE(denominator_power->right, nullptr);

    const auto* denominator_base =
        as_node<SymbolNode>(denominator_power->left.get());

    ASSERT_NE(denominator_base, nullptr);
    EXPECT_EQ(denominator_base->name, "x");

    const auto* denominator_exponent =
        as_node<NumberNode>(denominator_power->right.get());

    ASSERT_NE(denominator_exponent, nullptr);
    EXPECT_EQ(denominator_exponent->value, "2");

    // --------------------------------------------------------
    // Parcela: 20*pi
    // --------------------------------------------------------

    const auto* product =
        as_node<BinaryNode>(denominator->right.get());

    ASSERT_NE(product, nullptr);
    ASSERT_EQ(product->op, BinaryOp::Multiply);

    ASSERT_NE(product->left, nullptr);
    ASSERT_NE(product->right, nullptr);

    const auto* coefficient =
        as_node<NumberNode>(product->left.get());

    ASSERT_NE(coefficient, nullptr);
    EXPECT_EQ(coefficient->value, "20");

    const auto* pi =
        as_node<SymbolNode>(product->right.get());

    ASSERT_NE(pi, nullptr);
    EXPECT_EQ(pi->name, "pi");
}