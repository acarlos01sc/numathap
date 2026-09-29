#include <memory>

#include "numathap/numathap.hpp"
#include "test_framework2.hpp"
//#include "numathap/config/ComplexDoubleAdapter.hpp"
#include <iostream>

using namespace numathap;

namespace {

void expectComplex(
    const Value& value,
    double expectedReal,
    double expectedImaginary)
{
    EXPECT_TRUE(value.isComplex());

    const auto& z = value.complex();

    EXPECT_EQ(z.real().value(), expectedReal);
    EXPECT_EQ(z.imaginary().value(), expectedImaginary);
}

void expectComplexNear(
    const Value& value,
    double expectedReal,
    double expectedImaginary,
    double tolerance = 1e-10) {

    EXPECT_TRUE(value.isComplex());

    const auto& z = value.complex();

    const double realError =
        z.real().value() - expectedReal;

    const double imaginaryError =
        z.imaginary().value() - expectedImaginary;

    EXPECT_TRUE(
        realError < tolerance &&
        realError > -tolerance);

    EXPECT_TRUE(
        imaginaryError < tolerance &&
        imaginaryError > -tolerance);
}

MathEnvironment complexEnvironment()
{
    //return MathEnvironment(
    //    std::make_unique<config::ComplexDoubleAdapter>());
    return MathEnvironment(std::make_unique<config::ComplexDoubleAdapter>()); 
}

} // namespace

TEST(ComplexEvaluation, PowerOfOnePlusI)
{
    auto environment = complexEnvironment();
    auto expression = prepare("z^4", environment);

    Context context;
    context.setValue("z", "1+i");

    const auto result = evaluate_cx(expression, context);

    // (1+i)^4 = -4
    expectComplexNear(result, -4.0, 0.0);
}

TEST(ComplexEvaluation, Addition)
{
    auto environment = complexEnvironment();
    auto expression = prepare("z + w", environment);

    Context context;
    context.setValue("z", "2+3*i");
    context.setValue("w", "4-5*i");

    const auto result = evaluate_cx(expression, context);

    // (2+3i) + (4-5i) = 6-2i
    expectComplex(result, 6.0, -2.0);
}

TEST(ComplexEvaluation, Multiplication)
{
    auto environment = complexEnvironment();
    auto expression = prepare("z*w", environment);

    Context context;
    context.setValue("z", "2+3*i");
    context.setValue("w", "4-5*i");

    const auto result = evaluate_cx(expression, context);

    // (2+3i)(4-5i) = 23+2i
    expectComplex(result, 23.0, 2.0);
}

TEST(ComplexEvaluation, ImaginaryUnit)
{
    auto environment = complexEnvironment();
    auto expression = prepare("i*i", environment);

    Context context;

    const auto result = evaluate_cx(expression, context);

    expectComplex(result, -1.0, 0.0);
}

TEST(ComplexEvaluation, ImaginaryUnitAlias)
{
    auto environment = complexEnvironment();
    auto expression = prepare("j*j", environment);

    Context context;

    const auto result = evaluate_cx(expression, context);

    expectComplex(result, -1.0, 0.0);
}

TEST(ComplexEvaluation, SquareRootOfMinusOne)
{
    auto environment = complexEnvironment();
    auto expression = prepare("sqrt(-1)", environment);

    Context context;

    const auto result = evaluate_cx(expression, context);

    const auto& z = result.complex();

    std::cout << "sqrt(-1): real = "
              << z.real().value()
              << ", imaginary = "
              << z.imaginary().value()
              << '\n';

    expectComplexNear(result, 0.0, 1.0);
}

TEST(ComplexEvaluation, ComplexConstantFromContext) {
    auto environment = complexEnvironment();
    auto expression = prepare("z", environment);

    Context context;
    context.setValue("z", "1+i");

    const auto result = evaluate_cx(expression, context);

    const auto& z = result.complex();

    std::cout << "real = " << z.real().value()
              << ", imaginary = " << z.imaginary().value()
              << '\n';

    expectComplex(result, 1.0, 1.0);
}

TEST(ComplexEvaluation, AdapterSquareRootOfMinusOne)
{
    config::ComplexDoubleAdapter adapter;

    std::vector<Value> arguments;
    arguments.emplace_back(
        numeric::Complex(
            numeric::Real(-1.0),
            numeric::Real(0.0)));

    const auto result = adapter.callFunction("sqrt", arguments);

    const auto& z = result.complex();

    std::cout << "adapter sqrt(-1): real = "
              << z.real().value()
              << ", imaginary = "
              << z.imaginary().value()
              << '\n';
}

TEST(ComplexEvaluation, UnaryMinusPreservesImaginaryZero)
{
    auto environment = complexEnvironment();
    auto expression = prepare("-1", environment);

    Context context;

    const auto result = evaluate_cx(expression, context);

    const auto& z = result.complex();

    std::cout << "unary -1: real = "
              << z.real().value()
              << ", imaginary = "
              << z.imaginary().value()
              << '\n';
}

TEST(ComplexEvaluation, EulerRepresentation) {
    auto environment = complexEnvironment();
    auto expression = prepare("exp(i*pi)", environment);

    Context context;
    const auto result = evaluate_cx(expression, context);

    expectComplexNear(result, -1.0, 0.0);
}

TEST(ComplexEvaluation, EulerRepresentation2) {
    auto environment = complexEnvironment();
    auto expression = prepare("exp(i*pi/2)", environment);

    Context context;
    const auto result = evaluate_cx(expression, context);

    expectComplexNear(result, 0.0, 1.0);
}

int main()
{
    return testfw2::runFiltered("");
}