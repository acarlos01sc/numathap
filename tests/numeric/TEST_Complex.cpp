#include "numathap/config/ComplexDoubleAdapter.hpp"
#include "numathap/core/Value.hpp"
#include "numathap/numeric/Complex.hpp"

#include "test_framework2.hpp"

#include <array>
#include <cmath>
#include <numbers>
#include <stdexcept>

using numathap::config::ComplexDoubleAdapter;
using numathap::core::Value;
using numathap::numeric::Complex;
using numathap::numeric::Real;

// -----------------------------------------------------------------------------
// Test helpers
// -----------------------------------------------------------------------------

namespace {

using Arguments0 = std::array<Value, 0>;
using Arguments1 = std::array<Value, 1>;
using Arguments2 = std::array<Value, 2>;
using Arguments3 = std::array<Value, 3>;

constexpr double tolerance = 1.0e-14;

bool approximatelyEqual(double lhs, double rhs)
{
    return std::abs(lhs - rhs) < tolerance;
}

} // namespace

// -----------------------------------------------------------------------------
// Complex tests
// -----------------------------------------------------------------------------

TEST(Complex, DefaultConstructor)
{
    const Complex z;

    EXPECT_EQ(z.real(), Real(0.0));
    EXPECT_EQ(z.imaginary(), Real(0.0));
}

TEST(Complex, RealConstructor)
{
    const Complex z(Real(3.0));

    EXPECT_EQ(z.real(), Real(3.0));
    EXPECT_EQ(z.imaginary(), Real(0.0));
}

TEST(Complex, CartesianConstructor)
{
    const Complex z(Real(3.0), Real(4.0));

    EXPECT_EQ(z.real(), Real(3.0));
    EXPECT_EQ(z.imaginary(), Real(4.0));
}

TEST(Complex, Conjugate)
{
    const Complex z(Real(3.0), Real(4.0));

    const Complex conjugate = z.conjugate();

    EXPECT_EQ(conjugate.real(), Real(3.0));
    EXPECT_EQ(conjugate.imaginary(), Real(-4.0));
}

TEST(Complex, Norm)
{
    const Complex z(Real(3.0), Real(4.0));

    // |3 + 4i|² = 3² + 4² = 25
    EXPECT_EQ(z.norm(), Real(25.0));
}

TEST(Complex, Modulus)
{
    const Complex z(Real(3.0), Real(4.0));

    // |3 + 4i| = 5
    EXPECT_EQ(z.modulus(), Real(5.0));
}

TEST(Complex, Addition)
{
    const Complex z1(Real(3.0), Real(4.0));
    const Complex z2(Real(1.0), Real(2.0));

    const Complex result = z1 + z2;

    EXPECT_EQ(result.real(), Real(4.0));
    EXPECT_EQ(result.imaginary(), Real(6.0));
}

TEST(Complex, Subtraction)
{
    const Complex z1(Real(3.0), Real(4.0));
    const Complex z2(Real(1.0), Real(2.0));

    const Complex result = z1 - z2;

    EXPECT_EQ(result.real(), Real(2.0));
    EXPECT_EQ(result.imaginary(), Real(2.0));
}

TEST(Complex, Multiplication)
{
    const Complex z1(Real(3.0), Real(4.0));
    const Complex z2(Real(1.0), Real(2.0));

    // (3 + 4i)(1 + 2i) = -5 + 10i
    const Complex result = z1 * z2;

    EXPECT_EQ(result.real(), Real(-5.0));
    EXPECT_EQ(result.imaginary(), Real(10.0));
}

TEST(Complex, Division)
{
    const Complex z1(Real(3.0), Real(4.0));
    const Complex z2(Real(1.0), Real(2.0));

    // (3 + 4i)/(1 + 2i) = 11/5 - 2/5 i
    const Complex result = z1 / z2;

    EXPECT_EQ(result.real(), Real(11.0 / 5.0));
    EXPECT_EQ(result.imaginary(), Real(-2.0 / 5.0));
}

TEST(Complex, DivisionByZero)
{
    const Complex z1(Real(3.0), Real(4.0));
    const Complex zero;

    EXPECT_THROWS(z1 / zero, std::domain_error);
}

TEST(Complex, UnaryMinus)
{
    const Complex z(Real(3.0), Real(-4.0));

    const Complex result = -z;

    EXPECT_EQ(result.real(), Real(-3.0));
    EXPECT_EQ(result.imaginary(), Real(4.0));
}

TEST(Complex, Equality)
{
    const Complex z1(Real(3.0), Real(4.0));
    const Complex z2(Real(3.0), Real(4.0));
    const Complex z3(Real(3.0), Real(5.0));

    EXPECT_TRUE(z1 == z2);
    EXPECT_TRUE(z1 != z3);
}

TEST(Complex, ConjugateProductEqualsNorm)
{
    const Complex z(Real(3.0), Real(4.0));

    const Complex result = z * z.conjugate();

    EXPECT_EQ(result.real(), z.norm());
    EXPECT_EQ(result.imaginary(), Real(0.0));
}

// -----------------------------------------------------------------------------
// Value tests
// -----------------------------------------------------------------------------

TEST(Value, DefaultConstructor)
{
    const Value value;

    EXPECT_TRUE(value.isReal());
    EXPECT_EQ(value.real(), Real(0.0));
}

TEST(Value, RealConstructor)
{
    const Value value(Real(3.0));

    EXPECT_TRUE(value.isReal());
    EXPECT_TRUE(!value.isComplex());
    EXPECT_EQ(value.real(), Real(3.0));
}

TEST(Value, ComplexConstructor)
{
    const Complex z(Real(3.0), Real(4.0));

    const Value value(z);

    EXPECT_TRUE(!value.isReal());
    EXPECT_TRUE(value.isComplex());
    EXPECT_EQ(value.complex(), z);
}

TEST(Value, RealStorageConstructor)
{
    const Value value(3.0);

    EXPECT_TRUE(value.isReal());
    EXPECT_EQ(value.real(), Real(3.0));
}

TEST(Value, RealAddition)
{
    const Value lhs(Real(3.0));
    const Value rhs(Real(2.0));

    const Value result = lhs + rhs;

    EXPECT_TRUE(result.isReal());
    EXPECT_EQ(result.real(), Real(5.0));
}

TEST(Value, ComplexAddition)
{
    const Value lhs(Complex(Real(3.0), Real(4.0)));
    const Value rhs(Complex(Real(1.0), Real(2.0)));

    const Value result = lhs + rhs;

    EXPECT_TRUE(result.isComplex());
    EXPECT_EQ(result.complex(), Complex(Real(4.0), Real(6.0)));
}

TEST(Value, RealComplexAddition)
{
    const Value lhs(Real(3.0));
    const Value rhs(Complex(Real(1.0), Real(2.0)));

    const Value result = lhs + rhs;

    EXPECT_TRUE(result.isComplex());
    EXPECT_EQ(result.complex(), Complex(Real(4.0), Real(2.0)));
}

TEST(Value, ComplexRealAddition)
{
    const Value lhs(Complex(Real(3.0), Real(4.0)));
    const Value rhs(Real(1.0));

    const Value result = lhs + rhs;

    EXPECT_TRUE(result.isComplex());
    EXPECT_EQ(result.complex(), Complex(Real(4.0), Real(4.0)));
}

TEST(Value, Subtraction)
{
    const Value lhs(Complex(Real(3.0), Real(4.0)));
    const Value rhs(Real(1.0));

    const Value result = lhs - rhs;

    EXPECT_TRUE(result.isComplex());
    EXPECT_EQ(result.complex(), Complex(Real(2.0), Real(4.0)));
}

TEST(Value, Multiplication)
{
    const Value lhs(Real(2.0));
    const Value rhs(Complex(Real(3.0), Real(4.0)));

    const Value result = lhs * rhs;

    EXPECT_TRUE(result.isComplex());
    EXPECT_EQ(result.complex(), Complex(Real(6.0), Real(8.0)));
}

TEST(Value, Division)
{
    const Value lhs(Complex(Real(3.0), Real(4.0)));
    const Value rhs(Real(2.0));

    const Value result = lhs / rhs;

    EXPECT_TRUE(result.isComplex());
    EXPECT_EQ(result.complex(), Complex(Real(1.5), Real(2.0)));
}

TEST(Value, UnaryMinusReal)
{
    const Value value(Real(3.0));

    const Value result = -value;

    EXPECT_TRUE(result.isReal());
    EXPECT_EQ(result.real(), Real(-3.0));
}

TEST(Value, UnaryMinusComplex)
{
    const Value value(Complex(Real(3.0), Real(4.0)));

    const Value result = -value;

    EXPECT_TRUE(result.isComplex());
    EXPECT_EQ(
        result.complex(),
        Complex(Real(-3.0), Real(-4.0)));
}

TEST(Value, RealEquality)
{
    const Value lhs(Real(3.0));
    const Value rhs(Real(3.0));

    EXPECT_TRUE(lhs == rhs);
    EXPECT_TRUE(!(lhs != rhs));
}

TEST(Value, ComplexEquality)
{
    const Value lhs(Complex(Real(3.0), Real(4.0)));
    const Value rhs(Complex(Real(3.0), Real(4.0)));

    EXPECT_TRUE(lhs == rhs);
    EXPECT_TRUE(!(lhs != rhs));
}

TEST(Value, RealComplexEquality)
{
    const Value real(Real(3.0));
    const Value complex(Complex(Real(3.0), Real(0.0)));

    EXPECT_TRUE(real == complex);
    EXPECT_TRUE(!(real != complex));
}

TEST(Value, RealLessThan)
{
    const Value lhs(Real(2.0));
    const Value rhs(Real(3.0));

    EXPECT_TRUE(lhs < rhs);
    EXPECT_TRUE(!(rhs < lhs));
}

TEST(Value, RealLessThanOrEqual)
{
    const Value lhs(Real(2.0));
    const Value rhs(Real(2.0));

    EXPECT_TRUE(lhs <= rhs);
    EXPECT_TRUE(lhs >= rhs);
}

TEST(Value, RealGreaterThan)
{
    const Value lhs(Real(3.0));
    const Value rhs(Real(2.0));

    EXPECT_TRUE(lhs > rhs);
    EXPECT_TRUE(!(lhs < rhs));
}

TEST(Value, ComplexRelationalOrderingThrows)
{
    const Value real(Real(3.0));
    const Value complex(Complex(Real(2.0), Real(1.0)));

    EXPECT_THROWS(real < complex, std::domain_error);
    EXPECT_THROWS(real <= complex, std::domain_error);
    EXPECT_THROWS(real > complex, std::domain_error);
    EXPECT_THROWS(real >= complex, std::domain_error);
    EXPECT_THROWS(complex < real, std::domain_error);
}

TEST(Value, ComplexDivisionByZero)
{
    const Value value(Complex(Real(3.0), Real(4.0)));
    const Value zero{Complex{}};

    EXPECT_THROWS(value / zero, std::domain_error);
}

// -----------------------------------------------------------------------------
// ComplexDoubleAdapter tests
// -----------------------------------------------------------------------------

TEST(ComplexDoubleAdapter, Name)
{
    const ComplexDoubleAdapter adapter;

    EXPECT_EQ(adapter.name(), "complex-double");
}

TEST(ComplexDoubleAdapter, Clone)
{
    const ComplexDoubleAdapter adapter;

    const auto clone = adapter.clone();

    EXPECT_TRUE(clone != nullptr);
    EXPECT_EQ(clone->name(), "complex-double");
}

TEST(ComplexDoubleAdapter, ResolveI)
{
    const ComplexDoubleAdapter adapter;

    const Value result =
        adapter.resolveConstant("i");

    EXPECT_TRUE(result.isComplex());

    EXPECT_EQ(
        result.complex(),
        Complex(Real(0.0), Real(1.0)));
}

TEST(ComplexDoubleAdapter, ResolvePi)
{
    const ComplexDoubleAdapter adapter;

    const Value result =
        adapter.resolveConstant("pi");

    EXPECT_TRUE(result.isReal());

    EXPECT_TRUE(
        approximatelyEqual(
            result.real().value(),
            std::numbers::pi_v<double>));
}

TEST(ComplexDoubleAdapter, ResolveE)
{
    const ComplexDoubleAdapter adapter;

    const Value result =
        adapter.resolveConstant("e");

    EXPECT_TRUE(result.isReal());

    EXPECT_TRUE(
        approximatelyEqual(
            result.real().value(),
            std::numbers::e_v<double>));
}

TEST(ComplexDoubleAdapter, UnknownConstantThrows)
{
    const ComplexDoubleAdapter adapter;

    EXPECT_THROWS(
        adapter.resolveConstant("unknown"),
        std::invalid_argument);
}

// -----------------------------------------------------------------------------
// Elementary functions
// -----------------------------------------------------------------------------

TEST(ComplexDoubleAdapter, Exp)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(0.0), Real(1.0)));

    const Arguments1 arguments{z};

    const Value result =
        adapter.callFunction("exp", arguments);

    EXPECT_TRUE(result.isComplex());

    // exp(i) = cos(1) + i sin(1)
    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().real().value(),
            std::cos(1.0)));

    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().imaginary().value(),
            std::sin(1.0)));
}

TEST(ComplexDoubleAdapter, Sin)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(1.0), Real(0.5)));

    const Arguments1 arguments{z};

    const Value result =
        adapter.callFunction("sin", arguments);

    // sin(x + iy)
    // = sin(x) cosh(y) + i cos(x) sinh(y)
    const double expectedReal =
        std::sin(1.0) * std::cosh(0.5);

    const double expectedImaginary =
        std::cos(1.0) * std::sinh(0.5);

    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().real().value(),
            expectedReal));

    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().imaginary().value(),
            expectedImaginary));
}

TEST(ComplexDoubleAdapter, Cos)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(1.0), Real(0.5)));

    const Arguments1 arguments{z};

    const Value result =
        adapter.callFunction("cos", arguments);

    // cos(x + iy)
    // = cos(x) cosh(y) - i sin(x) sinh(y)
    const double expectedReal =
        std::cos(1.0) * std::cosh(0.5);

    const double expectedImaginary =
        -std::sin(1.0) * std::sinh(0.5);

    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().real().value(),
            expectedReal));

    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().imaginary().value(),
            expectedImaginary));
}

TEST(ComplexDoubleAdapter, Sinh)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(1.0), Real(0.5)));

    const Arguments1 arguments{z};

    const Value result =
        adapter.callFunction("sinh", arguments);

    // sinh(x + iy)
    // = sinh(x) cos(y) + i cosh(x) sin(y)
    const double expectedReal =
        std::sinh(1.0) * std::cos(0.5);

    const double expectedImaginary =
        std::cosh(1.0) * std::sin(0.5);

    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().real().value(),
            expectedReal));

    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().imaginary().value(),
            expectedImaginary));
}

TEST(ComplexDoubleAdapter, Conjugate)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(3.0), Real(4.0)));

    const Arguments1 arguments{z};

    const Value result =
        adapter.callFunction("conj", arguments);

    EXPECT_TRUE(result.isComplex());

    EXPECT_EQ(
        result.complex(),
        Complex(Real(3.0), Real(-4.0)));
}

TEST(ComplexDoubleAdapter, RealPart)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(3.0), Real(4.0)));

    const Arguments1 arguments{z};

    const Value result =
        adapter.callFunction("real", arguments);

    EXPECT_TRUE(result.isReal());
    EXPECT_EQ(result.real(), Real(3.0));
}

TEST(ComplexDoubleAdapter, ImaginaryPart)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(3.0), Real(4.0)));

    const Arguments1 arguments{z};

    const Value result =
        adapter.callFunction("imag", arguments);

    EXPECT_TRUE(result.isReal());
    EXPECT_EQ(result.real(), Real(4.0));
}

TEST(ComplexDoubleAdapter, Abs)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(3.0), Real(4.0)));

    const Arguments1 arguments{z};

    const Value result =
        adapter.callFunction("abs", arguments);

    EXPECT_TRUE(result.isReal());
    EXPECT_EQ(result.real(), Real(5.0));
}

TEST(ComplexDoubleAdapter, Norm)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(3.0), Real(4.0)));

    const Arguments1 arguments{z};

    const Value result =
        adapter.callFunction("norm", arguments);

    EXPECT_TRUE(result.isReal());
    EXPECT_EQ(result.real(), Real(25.0));
}

TEST(ComplexDoubleAdapter, Arg)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(0.0), Real(1.0)));

    const Arguments1 arguments{z};

    const Value result =
        adapter.callFunction("arg", arguments);

    EXPECT_TRUE(result.isReal());

    EXPECT_TRUE(
        approximatelyEqual(
            result.real().value(),
            std::numbers::pi_v<double> / 2.0));
}

// -----------------------------------------------------------------------------
// Multivalued functions
// -----------------------------------------------------------------------------

TEST(ComplexDoubleAdapter, LogPrincipalBranch)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(-1.0), Real(0.0)));

    const Arguments1 arguments{z};

    const Value result =
        adapter.callFunction("log", arguments);

    EXPECT_TRUE(result.isComplex());

    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().real().value(),
            0.0));

    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().imaginary().value(),
            std::numbers::pi_v<double>));
}

TEST(ComplexDoubleAdapter, LogPositiveBranch)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(1.0), Real(0.0)));

    const Value k(Real(1.0));

    const Arguments2 arguments{z, k};

    const Value result =
        adapter.callFunction("log", arguments);

    EXPECT_TRUE(result.isComplex());

    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().real().value(),
            0.0));

    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().imaginary().value(),
            2.0 * std::numbers::pi_v<double>));
}

TEST(ComplexDoubleAdapter, LogNegativeBranch)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(1.0), Real(0.0)));

    const Value k(Real(-1.0));

    const Arguments2 arguments{z, k};

    const Value result =
        adapter.callFunction("log", arguments);

    EXPECT_TRUE(result.isComplex());

    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().real().value(),
            0.0));

    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().imaginary().value(),
            -2.0 * std::numbers::pi_v<double>));
}

TEST(ComplexDoubleAdapter, SqrtPrincipalBranch)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(-1.0), Real(0.0)));

    const Arguments1 arguments{z};

    const Value result =
        adapter.callFunction("sqrt", arguments);

    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().real().value(),
            0.0));

    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().imaginary().value(),
            1.0));
}

TEST(ComplexDoubleAdapter, SqrtOddBranch)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(-1.0), Real(0.0)));

    const Value k(Real(1.0));

    const Arguments2 arguments{z, k};

    const Value result =
        adapter.callFunction("sqrt", arguments);

    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().real().value(),
            0.0));

    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().imaginary().value(),
            -1.0));
}

TEST(ComplexDoubleAdapter, SqrtEvenBranch)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(-1.0), Real(0.0)));

    const Value k(Real(2.0));

    const Arguments2 arguments{z, k};

    const Value result =
        adapter.callFunction("sqrt", arguments);

    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().real().value(),
            0.0));

    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().imaginary().value(),
            1.0));
}

TEST(ComplexDoubleAdapter, PowPrincipalBranch)
{
    const ComplexDoubleAdapter adapter;

    const Value base(
        Complex(Real(-1.0), Real(0.0)));

    const Value exponent(
        Complex(Real(0.5), Real(0.0)));

    const Arguments2 arguments{
        base,
        exponent
    };

    const Value result =
        adapter.callFunction("pow", arguments);

    // (-1)^(1/2) = i on the principal branch.
    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().real().value(),
            0.0));

    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().imaginary().value(),
            1.0));
}

TEST(ComplexDoubleAdapter, PowBranch)
{
    const ComplexDoubleAdapter adapter;

    const Value base(
        Complex(Real(-1.0), Real(0.0)));

    const Value exponent(
        Complex(Real(0.5), Real(0.0)));

    const Value k(Real(1.0));

    const Arguments3 arguments{
        base,
        exponent,
        k
    };

    const Value result =
        adapter.callFunction("pow", arguments);

    /*
     * (-1)^(1/2), branch k = 1:
     *
     * exp((1/2) * (i*pi + 2*pi*i))
     * = exp(3*pi*i/2)
     * = -i
     */
    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().real().value(),
            0.0));

    EXPECT_TRUE(
        approximatelyEqual(
            result.complex().imaginary().value(),
            -1.0));
}

// -----------------------------------------------------------------------------
// Argument validation
// -----------------------------------------------------------------------------

TEST(ComplexDoubleAdapter, RealArgumentThrows)
{
    const ComplexDoubleAdapter adapter;

    const Value real(Real(2.0));

    const Arguments1 arguments{real};

    EXPECT_THROWS(
        adapter.callFunction("exp", arguments),
        std::invalid_argument);
}

TEST(ComplexDoubleAdapter, UnknownFunctionThrows)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(1.0), Real(1.0)));

    const Arguments1 arguments{z};

    EXPECT_THROWS(
        adapter.callFunction("unknown", arguments),
        std::invalid_argument);
}

TEST(ComplexDoubleAdapter, WrongArgumentCountThrows)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(1.0), Real(1.0)));

    const Arguments0 emptyArguments{};
    const Arguments2 twoArguments{z, z};

    EXPECT_THROWS(
        adapter.callFunction("sin", emptyArguments),
        std::invalid_argument);

    EXPECT_THROWS(
        adapter.callFunction("sin", twoArguments),
        std::invalid_argument);
}

TEST(ComplexDoubleAdapter, NonIntegerBranchThrows)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(1.0), Real(0.0)));

    const Value k(Real(0.5));

    const Arguments2 logArguments{z, k};
    const Arguments2 sqrtArguments{z, k};

    const Arguments3 powArguments{
        z,
        z,
        k
    };

    EXPECT_THROWS(
        adapter.callFunction("log", logArguments),
        std::invalid_argument);

    EXPECT_THROWS(
        adapter.callFunction("sqrt", sqrtArguments),
        std::invalid_argument);

    EXPECT_THROWS(
        adapter.callFunction("pow", powArguments),
        std::invalid_argument);
}

TEST(ComplexDoubleAdapter, ComplexBranchThrows)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(1.0), Real(0.0)));

    const Value k(
        Complex(Real(1.0), Real(0.0)));

    const Arguments2 arguments{z, k};

    EXPECT_THROWS(
        adapter.callFunction("log", arguments),
        std::invalid_argument);
}

TEST(ComplexDoubleAdapter, InvalidLogArgumentCountThrows)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(1.0), Real(0.0)));

    const Arguments3 arguments{z, z, z};

    EXPECT_THROWS(
        adapter.callFunction("log", arguments),
        std::invalid_argument);
}

TEST(ComplexDoubleAdapter, InvalidSqrtArgumentCountThrows)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(1.0), Real(0.0)));

    const Arguments3 arguments{z, z, z};

    EXPECT_THROWS(
        adapter.callFunction("sqrt", arguments),
        std::invalid_argument);
}

TEST(ComplexDoubleAdapter, InvalidPowArgumentCountThrows)
{
    const ComplexDoubleAdapter adapter;

    const Value z(
        Complex(Real(1.0), Real(0.0)));

    const Arguments1 oneArgument{z};

    const Arguments3 threeArguments{z, z, z};

    const Arguments3 fourArguments{z, z, z};

    EXPECT_THROWS(
        adapter.callFunction("pow", oneArgument),
        std::invalid_argument);

    /*
     * pow accepts exactly 2 or 3 arguments.
     *
     * Four arguments are represented separately below because the
     * current helper aliases only cover up to three elements.
     */
    const std::array<Value, 4> four{
        z,
        z,
        z,
        z
    };

    (void)threeArguments;
    (void)fourArguments;

    EXPECT_THROWS(
        adapter.callFunction("pow", four),
        std::invalid_argument);
}

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------

int main()
{
    return testfw2::runAll();
}