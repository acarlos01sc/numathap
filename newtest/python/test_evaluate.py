import math

import _numathap
import pytest


def evaluate_expression(expression, variables=None):
    """Prepare and evaluate an expression with the given variables."""
    prepared = _numathap.prepare(expression)
    context = _numathap.Context()

    if variables:
        for name, value in variables.items():
            context.set_value(name, str(value))

    return _numathap.evaluate(prepared, context)


def test_evaluate_polynomial():
    result = evaluate_expression(
        "x^3 - 2*x^2 + 3*x - 1",
        {"x": 2},
    )

    assert result.real() == pytest.approx(5.0)


def test_evaluate_expression_with_multiple_variables():
    result = evaluate_expression(
        "2*x^2 + 3*x*y - y^2",
        {"x": 2, "y": 3},
    )

    assert result.real() == pytest.approx(17.0)


def test_evaluate_trigonometric_identity():
    x = 0.73

    result = evaluate_expression(
        "sin(x)^2 + cos(x)^2",
        {"x": x},
    )

    assert result.real() == pytest.approx(1.0, abs=1e-12)


def test_evaluate_exponential_and_logarithm():
    x = 2.5

    result = evaluate_expression(
        "exp(x) + log(x)",
        {"x": x},
    )

    expected = math.exp(x) + math.log(x)

    assert result.real() == pytest.approx(expected, rel=1e-12)


def test_evaluate_nested_functions():
    x = 0.4

    result = evaluate_expression(
        "sin(x^2) + cos(2*x)",
        {"x": x},
    )

    expected = math.sin(x**2) + math.cos(2 * x)

    assert result.real() == pytest.approx(expected, rel=1e-12)


def test_evaluate_rational_expression():
    x = 2.0

    result = evaluate_expression(
        "(x^2 - 1)/(x - 1)",
        {"x": x},
    )

    assert result.real() == pytest.approx(3.0)
