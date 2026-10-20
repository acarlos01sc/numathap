import _numathap
import pytest


@pytest.fixture(
    params=[
        _numathap.Algorithm.AdaptiveSimpson,
        _numathap.Algorithm.GaussKronrod15,
    ],
    ids=["adaptive_simpson", "gauss_kronrod15"],
)
def integration_environment(request):
    return _numathap.configure(request.param)


def integrate_expression(expression, lower, upper, environment):
    """Prepare and integrate an expression over a given interval."""
    prepared = _numathap.prepare(expression)

    context = _numathap.Context()
    context.set_interval("x", lower, upper)

    return _numathap.integrate(
        prepared,
        "x",
        context,
        environment,
    )


def test_integrate_polynomial(integration_environment):
    result = integrate_expression(
        "x^2",
        "0",
        "3",
        integration_environment,
    )

    assert result.real() == pytest.approx(9.0, abs=1e-8)


def test_integrate_trigonometric_function(integration_environment):
    result = integrate_expression(
        "sin(x)",
        "0",
        "pi",
        integration_environment,
    )

    assert result.real() == pytest.approx(2.0, abs=1e-8)


def test_integrate_with_reversed_bounds(integration_environment):
    result = integrate_expression(
        "x",
        "2",
        "0",
        integration_environment,
    )

    assert result.real() == pytest.approx(-2.0, abs=1e-8)


def test_integrate_gaussian(integration_environment):
    result = integrate_expression(
        "exp(-x^2)",
        "-5",
        "5",
        integration_environment,
    )

    assert result.real() == pytest.approx(
        1.772453850905516,
        abs=1e-8,
    )


def test_integrate_expression_with_parameter(integration_environment):
    prepared = _numathap.prepare("x^2 + a")

    context = _numathap.Context()
    context.set_interval("x", "0", "1")
    context.set_value("a", "2")

    result = _numathap.integrate(
        prepared,
        "x",
        context,
        integration_environment,
    )

    assert result.real() == pytest.approx(7.0 / 3.0, abs=1e-8)


def test_integrate_improper_gaussian(integration_environment):
    result = integrate_expression(
        "exp(-x^2)/sqrt(pi)",
        "-inf",
        "inf",
        integration_environment,
    )

    assert result.real() == pytest.approx(
        1.0,
        abs=1e-8,
    )
