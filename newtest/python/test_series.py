import _numathap
import pytest


@pytest.mark.parametrize(
    "order",
    [10, 20],
    ids=["10_terms", "20_terms"],
)
def test_taylor_sin(order):
    config = _numathap.TaylorConfig()
    config.order = order

    environment = _numathap.configure(config)

    expression = _numathap.prepare("sin(x)")
    expansion = _numathap.series(
        expression,
        "x",
        "0",
        environment,
    )

    context = _numathap.Context()
    context.set_value("x", "0.1")

    result = _numathap.evaluate(expansion, context)

    assert result.real() == pytest.approx(
        0.09983341664682815,
        abs=1e-8,
    )


def test_taylor_order_affects_result():
    expression = _numathap.prepare("exp(x)")

    context = _numathap.Context()
    context.set_value("x", "2")

    expected_results = {
        2: 5.0,
        5: 7.266666666666667,
    }

    for order, expected in expected_results.items():
        config = _numathap.TaylorConfig()
        config.order = order

        environment = _numathap.configure(config)

        expansion = _numathap.series(
            expression,
            "x",
            "0",
            environment,
        )

        result = _numathap.evaluate(expansion, context)

        assert result.real() == pytest.approx(
            expected,
            abs=1e-8,
        )


@pytest.mark.parametrize(
    "order, expected",
    [
        (10, 1.28111616),
        (20, 1.3291223452022211),
    ],
    ids=["order_10", "order_20"],
)
def test_taylor_x_over_sqrt_one_minus_x_squared(order, expected):
    expression = _numathap.prepare("x/sqrt(1-x^2)")

    config = _numathap.TaylorConfig()
    config.order = order
    environment = _numathap.configure(config)

    expansion = _numathap.series(
        expression,
        "x",
        "0",
        environment,
    )

    context = _numathap.Context()
    context.set_value("x", "0.8")

    result = _numathap.evaluate(expansion, context)

    assert result.real() == pytest.approx(
        expected,
        abs=1e-8,
    )


@pytest.mark.parametrize(
    "order, expected",
    [
        (10, 0.41449145129447619),
        (20, 0.41423972680682949),
    ],
    ids=["order_10", "order_20"],
)
def test_taylor_reciprocal_trigonometric_expression(order, expected):
    expression = _numathap.prepare("1/(1+cos(x)+sin(x))")

    config = _numathap.TaylorConfig()
    config.order = order
    environment = _numathap.configure(config)

    expansion = _numathap.series(
        expression,
        "x",
        "0",
        environment,
    )

    context = _numathap.Context()
    context.set_value("x", "0.8")

    result = _numathap.evaluate(expansion, context)

    assert result.real() == pytest.approx(
        expected,
        abs=1e-8,
    )


@pytest.mark.parametrize(
    "expression, expected",
    [
        ("asin(x)", 0.1001674211615598),
        ("acos(x)", 1.4706289056333368),
        ("atan(x)", 0.09966865249116204),
    ],
    ids=["asin", "acos", "atan"],
)
@pytest.mark.parametrize(
    "order",
    [10, 20],
    ids=["10_terms", "20_terms"],
)
def test_taylor_inverse_trigonometric_functions(expression, expected, order):
    config = _numathap.TaylorConfig()
    config.order = order
    environment = _numathap.configure(config)

    prepared_expression = _numathap.prepare(expression)
    expansion = _numathap.series(
        prepared_expression,
        "x",
        "0",
        environment,
    )

    context = _numathap.Context()
    context.set_value("x", "0.1")
    result = _numathap.evaluate(expansion, context)

    assert result.real() == pytest.approx(
        expected,
        abs=1e-8,
    )
