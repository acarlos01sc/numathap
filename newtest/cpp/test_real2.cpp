#include <gtest/gtest.h>

#include <numathap/numeric/Real2.hpp>

#include <stdexcept>

using numathap::numeric::Real2;

//----------------------------------------------------------
// Construction and conversion
//----------------------------------------------------------

TEST(Real2Construction, DefaultInitializesToZero) {
    const Real2 value;

    EXPECT_EQ(value, Real2(0));
    EXPECT_EQ(value.toString(), "0");
}

TEST(Real2Construction, ConstructsFromInteger) {
    const Real2 value(42);

    EXPECT_EQ(value, Real2("42"));
}

TEST(Real2Construction, ConstructsFromString) {
    const Real2 value("123.456");

    EXPECT_EQ(value.toString(), "123.456");
}

TEST(Real2Construction, PreservesAvailablePrecision) {
    const Real2 value("1.234567890123456");

    EXPECT_EQ(value.toString(), "1.234567890123456");
}

TEST(Real2Construction, RejectsEmptyString) {
    EXPECT_THROW(Real2(""), std::invalid_argument);
}

TEST(Real2Construction, RejectsInvalidString) {
    EXPECT_THROW(Real2("abc"), std::invalid_argument);
}

TEST(Real2Construction, RejectsTrailingCharacters) {
    EXPECT_THROW(Real2("123abc"), std::invalid_argument);
}

//----------------------------------------------------------
// Arithmetic operations
//----------------------------------------------------------

TEST(Real2Arithmetic, Addition) {
    EXPECT_EQ(Real2(10) + Real2(5), Real2(15));
}

TEST(Real2Arithmetic, Subtraction) {
    EXPECT_EQ(Real2(10) - Real2(5), Real2(5));
}

TEST(Real2Arithmetic, Multiplication) {
    EXPECT_EQ(Real2(10) * Real2(5), Real2(50));
}

TEST(Real2Arithmetic, Division) {
    EXPECT_EQ(Real2(10) / Real2(5), Real2(2));
}

TEST(Real2Arithmetic, UnaryNegation) {
    EXPECT_EQ(-Real2(10), Real2(-10));
}

TEST(Real2Arithmetic, CombinedOperations) {
    const Real2 result =
        (Real2(10) + Real2(5)) * Real2(2) / Real2(3);

    EXPECT_EQ(result, Real2(10));
}

//----------------------------------------------------------
// Comparison operators
//----------------------------------------------------------

TEST(Real2Comparison, Equality) {
    EXPECT_TRUE(Real2(10) == Real2(10));
    EXPECT_FALSE(Real2(10) == Real2(5));
}

TEST(Real2Comparison, Inequality) {
    EXPECT_TRUE(Real2(10) != Real2(5));
    EXPECT_FALSE(Real2(10) != Real2(10));
}

TEST(Real2Comparison, LessThan) {
    EXPECT_TRUE(Real2(5) < Real2(10));
    EXPECT_FALSE(Real2(10) < Real2(5));
}

TEST(Real2Comparison, LessThanOrEqual) {
    EXPECT_TRUE(Real2(5) <= Real2(10));
    EXPECT_TRUE(Real2(10) <= Real2(10));
}

TEST(Real2Comparison, GreaterThan) {
    EXPECT_TRUE(Real2(10) > Real2(5));
    EXPECT_FALSE(Real2(5) > Real2(10));
}

TEST(Real2Comparison, GreaterThanOrEqual) {
    EXPECT_TRUE(Real2(10) >= Real2(5));
    EXPECT_TRUE(Real2(10) >= Real2(10));
}

//----------------------------------------------------------
// Absolute value and sign
//----------------------------------------------------------

TEST(Real2Operations, AbsoluteValue) {
    EXPECT_EQ(Real2(-10).abs(), Real2(10));
    EXPECT_EQ(Real2(10).abs(), Real2(10));
}

TEST(Real2Operations, AbsoluteValueNormalizesNegativeZero) {
    const Real2 negative_zero("-0.0");
    const Real2 result = negative_zero.abs();

    EXPECT_EQ(result, Real2(0));
    EXPECT_EQ(result.toString(), "0");
}

TEST(Real2Operations, SignOfPositiveNumber) {
    EXPECT_EQ(Real2(10).sign(), 1);
}

TEST(Real2Operations, SignOfNegativeNumber) {
    EXPECT_EQ(Real2(-10).sign(), -1);
}

TEST(Real2Operations, SignOfZero) {
    EXPECT_EQ(Real2(0).sign(), 0);
}

TEST(Real2Errors, SignOfNaN) {
    const Real2 nan = Real2(0) / Real2(0);

    EXPECT_THROW(nan.sign(), std::domain_error);
}

TEST(Real2Errors, SignOfPositiveInfinity) {
    const Real2 infinity = Real2(1) / Real2(0);

    EXPECT_THROW(infinity.sign(), std::domain_error);
}

TEST(Real2Errors, SignOfNegativeInfinity) {
    const Real2 infinity = Real2(-1) / Real2(0);

    EXPECT_THROW(infinity.sign(), std::domain_error);
}

//----------------------------------------------------------
// Numeric properties
//----------------------------------------------------------

TEST(Real2Properties, FiniteValues) {
    EXPECT_TRUE(Real2(0).isFinite());
    EXPECT_TRUE(Real2("123.456").isFinite());
    EXPECT_TRUE(Real2("-123.456").isFinite());
}

TEST(Real2Properties, NaNIsNotFinite) {
    const Real2 nan = Real2(0) / Real2(0);

    EXPECT_FALSE(nan.isFinite());
}

TEST(Real2Properties, InfinityIsNotFinite) {
    const Real2 infinity = Real2(1) / Real2(0);

    EXPECT_FALSE(infinity.isFinite());
}

TEST(Real2Properties, DetectsIntegerValues) {
    EXPECT_TRUE(Real2(0).isInteger());
    EXPECT_TRUE(Real2(10).isInteger());
    EXPECT_TRUE(Real2(-10).isInteger());
    EXPECT_TRUE(Real2("123456789").isInteger());
}

TEST(Real2Properties, DetectsNonIntegerValues) {
    EXPECT_FALSE(Real2("1.5").isInteger());
    EXPECT_FALSE(Real2("-2.75").isInteger());
}

TEST(Real2Properties, DetectsLargeIntegerValues) {
    const Real2 value("1e30");

    EXPECT_TRUE(value.isFinite());
    EXPECT_TRUE(value.isInteger());
}

TEST(Real2Properties, NonFiniteValuesAreNotIntegers) {
    const Real2 nan = Real2(0) / Real2(0);
    const Real2 infinity = Real2(1) / Real2(0);

    EXPECT_FALSE(nan.isInteger());
    EXPECT_FALSE(infinity.isInteger());
}

TEST(Real2Properties, MachineEpsilon) {
    const Real2 epsilon = Real2::epsilon();

    EXPECT_TRUE(epsilon.isFinite());
    EXPECT_GT(epsilon, Real2(0));
    EXPECT_LT(epsilon, Real2("1e-10"));
}

//----------------------------------------------------------
// String representation
//----------------------------------------------------------

TEST(Real2Conversion, ConvertsToString) {
    EXPECT_EQ(Real2(42).toString(), "42");
    EXPECT_EQ(Real2("3.125").toString(), "3.125");
    EXPECT_EQ(Real2(-10).toString(), "-10");
}


//----------------------------------------------------------
// Overflow
//----------------------------------------------------------
TEST(Real2Overflow, AdditionProducesInfinity) {
    const Real2 a("1.7e308");
    const Real2 b("1.7e308");

    const Real2 result = a + b;

    EXPECT_FALSE(result.isFinite());
}

TEST(Real2Overflow, MultiplicationProducesInfinity) {
    const Real2 a("1e200");
    const Real2 b("1e200");

    const Real2 result = a * b;

    EXPECT_FALSE(result.isFinite());
}

TEST(Real2Overflow, NegativeOverflowProducesInfinity) {
    const Real2 a("-1.7e308");
    const Real2 b("1.7e308");

    const Real2 result = a * b;

    EXPECT_FALSE(result.isFinite());
}

//----------------------------------------------------------
// Underflow
//----------------------------------------------------------
TEST(Real2Underflow, PreservesSubnormalResult) {
    const Real2 a("1e-300");
    const Real2 b("1e-20");

    const Real2 result = a * b;

    EXPECT_TRUE(result.isFinite());
    EXPECT_GT(result, Real2(0));
}

TEST(Real2Underflow, VerySmallResultRoundsToZero) {
    const Real2 a("1e-300");
    const Real2 b("1e-100");

    const Real2 result = a * b;

    EXPECT_TRUE(result.isFinite());
    EXPECT_EQ(result, Real2(0));
}


//----------------------------------------------------------
// NaN
//----------------------------------------------------------
TEST(Real2NaN, PropagatesThroughArithmetic) {
    const Real2 nan = Real2(0) / Real2(0);
    const Real2 value(10);

    EXPECT_FALSE((nan + value).isFinite());
    EXPECT_FALSE((nan - value).isFinite());
    EXPECT_FALSE((nan * value).isFinite());
    EXPECT_FALSE((nan / value).isFinite());
    EXPECT_FALSE((-nan).isFinite());
}

TEST(Real2NaNComparison, EqualityAndInequality) {
    const Real2 nan = Real2(0) / Real2(0);
    const Real2 value(10);

    EXPECT_FALSE(nan == value);
    EXPECT_TRUE(nan != value);

    EXPECT_FALSE(value == nan);
    EXPECT_TRUE(value != nan);
}

TEST(Real2NaNComparison, OrderedComparisons) {
    const Real2 nan = Real2(0) / Real2(0);
    const Real2 value(10);

    EXPECT_FALSE(nan < value);
    EXPECT_FALSE(nan <= value);
    EXPECT_FALSE(nan > value);
    EXPECT_FALSE(nan >= value);

    EXPECT_FALSE(value < nan);
    EXPECT_FALSE(value <= nan);
    EXPECT_FALSE(value > nan);
    EXPECT_FALSE(value >= nan);
}

TEST(Real2NaNComparison, NaNComparedWithItself) {
    const Real2 nan = Real2(0) / Real2(0);

    EXPECT_FALSE(nan == nan);
    EXPECT_TRUE(nan != nan);

    EXPECT_FALSE(nan < nan);
    EXPECT_FALSE(nan <= nan);
    EXPECT_FALSE(nan > nan);
    EXPECT_FALSE(nan >= nan);
}

TEST(Real2SquareRoot, PositiveNumber) {
    const Real2 value(25);

    EXPECT_EQ(value.sqrt(), Real2(5));
}

TEST(Real2SquareRoot, Zero) {
    const Real2 value(0);

    EXPECT_EQ(value.sqrt(), Real2(0));
}

TEST(Real2SquareRoot, NonPerfectSquare) {
    const Real2 value(2);
    const Real2 result = value.sqrt();

    const Real2 error = (result * result - value).abs();

    EXPECT_TRUE(result.isFinite());
    EXPECT_GT(result, Real2(0));
    EXPECT_LT(error, Real2("1e-15"));
}