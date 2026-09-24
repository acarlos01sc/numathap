#include "numathap/core/Value.hpp"
#include "numathap/numeric/Complex.hpp"
#include "test_framework.hpp"

#include <stdexcept>

using numathap::core::Value;
using numathap::numeric::Complex;
using numathap::numeric::Real;

// -----------------------------------------------------------------------------
// Complex tests
// -----------------------------------------------------------------------------

TEST(Complex, DefaultConstructor) {
    const Complex z;

    EXPECT_EQ(z.real(), Real(0.0));
    EXPECT_EQ(z.imaginary(), Real(0.0));
}

TEST(Complex, RealConstructor) {
    const Complex z(Real(3.0));

    EXPECT_EQ(z.real(), Real(3.0));
    EXPECT_EQ(z.imaginary(), Real(0.0));
}

TEST(Complex, CartesianConstructor) {
    const Complex z(Real(3.0), Real(4.0));

    EXPECT_EQ(z.real(), Real(3.0));
    EXPECT_EQ(z.imaginary(), Real(4.0));
}

TEST(Complex, Conjugate) {
    const Complex z(Real(3.0), Real(4.0));
    const Complex conjugate = z.conjugate();

    EXPECT_EQ(conjugate.real(), Real(3.0));
    EXPECT_EQ(conjugate.imaginary(), Real(-4.0));
}

TEST(Complex, Norm) {
    const Complex z(Real(3.0), Real(4.0));

    // |3 + 4i|² = 3² + 4² = 25
    EXPECT_EQ(z.norm(), Real(25.0));
}

TEST(Complex, Modulus) {
    const Complex z(Real(3.0), Real(4.0));

    // |3 + 4i| = 5
    EXPECT_EQ(z.modulus(), Real(5.0));
}

TEST(Complex, Addition) {
    const Complex z1(Real(3.0), Real(4.0));
    const Complex z2(Real(1.0), Real(2.0));

    const Complex result = z1 + z2;

    EXPECT_EQ(result.real(), Real(4.0));
    EXPECT_EQ(result.imaginary(), Real(6.0));
}

TEST(Complex, Subtraction) {
    const Complex z1(Real(3.0), Real(4.0));
    const Complex z2(Real(1.0), Real(2.0));

    const Complex result = z1 - z2;

    EXPECT_EQ(result.real(), Real(2.0));
    EXPECT_EQ(result.imaginary(), Real(2.0));
}

TEST(Complex, Multiplication) {
    const Complex z1(Real(3.0), Real(4.0));
    const Complex z2(Real(1.0), Real(2.0));

    // (3 + 4i)(1 + 2i) = -5 + 10i
    const Complex result = z1 * z2;

    EXPECT_EQ(result.real(), Real(-5.0));
    EXPECT_EQ(result.imaginary(), Real(10.0));
}

TEST(Complex, Division) {
    const Complex z1(Real(3.0), Real(4.0));
    const Complex z2(Real(1.0), Real(2.0));

    // (3 + 4i)/(1 + 2i) = 11/5 - 2/5 i
    const Complex result = z1 / z2;

    EXPECT_EQ(result.real(), Real(11.0 / 5.0));
    EXPECT_EQ(result.imaginary(), Real(-2.0 / 5.0));
}

TEST(Complex, DivisionByZero) {
    const Complex z1(Real(3.0), Real(4.0));
    const Complex zero;

    EXPECT_THROWS(z1 / zero, std::domain_error);
}

TEST(Complex, UnaryMinus) {
    const Complex z(Real(3.0), Real(-4.0));

    const Complex result = -z;

    EXPECT_EQ(result.real(), Real(-3.0));
    EXPECT_EQ(result.imaginary(), Real(4.0));
}

TEST(Complex, Equality) {
    const Complex z1(Real(3.0), Real(4.0));
    const Complex z2(Real(3.0), Real(4.0));
    const Complex z3(Real(3.0), Real(5.0));

    EXPECT_TRUE(z1 == z2);
    EXPECT_TRUE(z1 != z3);
}

TEST(Complex, ConjugateProductEqualsNorm) {
    const Complex z(Real(3.0), Real(4.0));

    const Complex result = z * z.conjugate();

    EXPECT_EQ(result.real(), z.norm());
    EXPECT_EQ(result.imaginary(), Real(0.0));
}

// -----------------------------------------------------------------------------
// Value tests
// -----------------------------------------------------------------------------

TEST(Value, DefaultConstructor) {
    const Value value;

    EXPECT_TRUE(value.isReal());
    EXPECT_EQ(value.real(), Real(0.0));
}

TEST(Value, RealConstructor) {
    const Value value(Real(3.0));

    EXPECT_TRUE(value.isReal());
    EXPECT_TRUE(!value.isComplex());
    EXPECT_EQ(value.real(), Real(3.0));
}

TEST(Value, ComplexConstructor) {
    const Complex z(Real(3.0), Real(4.0));
    const Value value(z);

    EXPECT_TRUE(!value.isReal());
    EXPECT_TRUE(value.isComplex());
    EXPECT_EQ(value.complex(), z);
}

TEST(Value, RealStorageConstructor) {
    const Value value(3.0);

    EXPECT_TRUE(value.isReal());
    EXPECT_EQ(value.real(), Real(3.0));
}

TEST(Value, RealAddition) {
    const Value lhs(Real(3.0));
    const Value rhs(Real(2.0));

    const Value result = lhs + rhs;

    EXPECT_TRUE(result.isReal());
    EXPECT_EQ(result.real(), Real(5.0));
}

TEST(Value, ComplexAddition) {
    const Value lhs(Complex(Real(3.0), Real(4.0)));
    const Value rhs(Complex(Real(1.0), Real(2.0)));

    const Value result = lhs + rhs;

    EXPECT_TRUE(result.isComplex());
    EXPECT_EQ(result.complex(), Complex(Real(4.0), Real(6.0)));
}

TEST(Value, RealComplexAddition) {
    const Value lhs(Real(3.0));
    const Value rhs(Complex(Real(1.0), Real(2.0)));

    const Value result = lhs + rhs;

    EXPECT_TRUE(result.isComplex());
    EXPECT_EQ(result.complex(), Complex(Real(4.0), Real(2.0)));
}

TEST(Value, ComplexRealAddition) {
    const Value lhs(Complex(Real(3.0), Real(4.0)));
    const Value rhs(Real(1.0));

    const Value result = lhs + rhs;

    EXPECT_TRUE(result.isComplex());
    EXPECT_EQ(result.complex(), Complex(Real(4.0), Real(4.0)));
}

TEST(Value, Subtraction) {
    const Value lhs(Complex(Real(3.0), Real(4.0)));
    const Value rhs(Real(1.0));

    const Value result = lhs - rhs;

    EXPECT_TRUE(result.isComplex());
    EXPECT_EQ(result.complex(), Complex(Real(2.0), Real(4.0)));
}

TEST(Value, Multiplication) {
    const Value lhs(Real(2.0));
    const Value rhs(Complex(Real(3.0), Real(4.0)));

    const Value result = lhs * rhs;

    EXPECT_TRUE(result.isComplex());
    EXPECT_EQ(result.complex(), Complex(Real(6.0), Real(8.0)));
}

TEST(Value, Division) {
    const Value lhs(Complex(Real(3.0), Real(4.0)));
    const Value rhs(Real(2.0));

    const Value result = lhs / rhs;

    EXPECT_TRUE(result.isComplex());
    EXPECT_EQ(result.complex(), Complex(Real(1.5), Real(2.0)));
}

TEST(Value, UnaryMinusReal) {
    const Value value(Real(3.0));

    const Value result = -value;

    EXPECT_TRUE(result.isReal());
    EXPECT_EQ(result.real(), Real(-3.0));
}

TEST(Value, UnaryMinusComplex) {
    const Value value(Complex(Real(3.0), Real(4.0)));

    const Value result = -value;

    EXPECT_TRUE(result.isComplex());
    EXPECT_EQ(result.complex(), Complex(Real(-3.0), Real(-4.0)));
}

TEST(Value, RealEquality) {
    const Value lhs(Real(3.0));
    const Value rhs(Real(3.0));

    EXPECT_TRUE(lhs == rhs);
    EXPECT_TRUE(!(lhs != rhs));
}

TEST(Value, ComplexEquality) {
    const Value lhs(Complex(Real(3.0), Real(4.0)));
    const Value rhs(Complex(Real(3.0), Real(4.0)));

    EXPECT_TRUE(lhs == rhs);
    EXPECT_TRUE(!(lhs != rhs));
}

TEST(Value, RealComplexEquality) {
    const Value real(Real(3.0));
    const Value complex(Complex(Real(3.0), Real(0.0)));

    EXPECT_TRUE(real == complex);
    EXPECT_TRUE(!(real != complex));
}

TEST(Value, RealLessThan) {
    const Value lhs(Real(2.0));
    const Value rhs(Real(3.0));

    EXPECT_TRUE(lhs < rhs);
    EXPECT_TRUE(!(rhs < lhs));
}

TEST(Value, RealLessThanOrEqual) {
    const Value lhs(Real(2.0));
    const Value rhs(Real(2.0));

    EXPECT_TRUE(lhs <= rhs);
    EXPECT_TRUE(lhs >= rhs);
}

TEST(Value, RealGreaterThan) {
    const Value lhs(Real(3.0));
    const Value rhs(Real(2.0));

    EXPECT_TRUE(lhs > rhs);
    EXPECT_TRUE(!(lhs < rhs));
}

TEST(Value, ComplexRelationalOrderingThrows) {
    const Value real(Real(3.0));
    const Value complex(Complex(Real(2.0), Real(1.0)));

    EXPECT_THROWS(real < complex, std::domain_error);
    EXPECT_THROWS(real <= complex, std::domain_error);
    EXPECT_THROWS(real > complex, std::domain_error);
    EXPECT_THROWS(real >= complex, std::domain_error);
    EXPECT_THROWS(complex < real, std::domain_error);
}

TEST(Value, ComplexDivisionByZero) {
    const Value value(Complex(Real(3.0), Real(4.0)));
    const Value zero{Complex{}};

    EXPECT_THROWS(value / zero, std::domain_error);
}

int main() {
    return testfw::runAll();
}