import _numathap
import pytest


def differentiate_expression(expression, variable, variable_value):
    """Differentiate and evaluate an expression at a given point."""
    prepared = _numathap.prepare(expression)
    derivative = _numathap.differentiate(prepared, variable)

    context = _numathap.Context()
    context.set_value(variable, variable_value)

    return _numathap.evaluate(derivative, context)


def test_differentiate_polynomial():
    result = differentiate_expression("x^3", "x", "2")

    assert result.real() == pytest.approx(12.0, abs=1e-8)


def test_differentiate_polynomial_sum():
    result = differentiate_expression(
        "x^3+2*x^2-5*x+7",
        "x",
        "2",
    )

    assert result.real() == pytest.approx(15.0, abs=1e-8)


def test_differentiate_sine():
    result = differentiate_expression("sin(x)", "x", "0")

    assert result.real() == pytest.approx(1.0, abs=1e-8)


def test_differentiate_chain_rule():
    result = differentiate_expression("sin(2*x)", "x", "0")

    assert result.real() == pytest.approx(2.0, abs=1e-8)


def test_differentiate_reciprocal():
    result = differentiate_expression("1/x", "x", "2")

    assert result.real() == pytest.approx(-0.25, abs=1e-8)


def test_differentiate_exponential():
    result = differentiate_expression("exp(x)", "x", "0")

    assert result.real() == pytest.approx(1.0, abs=1e-8)


def test_differentiate_logarithm():
    result = differentiate_expression("log(x)", "x", "1")

    assert result.real() == pytest.approx(1.0, abs=1e-8)


def test_differentiate_product_rule():
    result = differentiate_expression("x^2*sin(x)", "x", "0")

    assert result.real() == pytest.approx(0.0, abs=1e-8)
