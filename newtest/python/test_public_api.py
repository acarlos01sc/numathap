import _numathap


def test_module_can_be_imported():
    assert _numathap is not None


def test_prepare_returns_prepared_ast():
    expression = "x^2"

    prepared = _numathap.prepare(expression)

    assert prepared is not None
    assert prepared.expression() == expression
    assert not prepared.empty()


def test_prepare_preserves_expression():
    expression = "sin(x) + 2*x"

    prepared = _numathap.prepare(expression)

    assert prepared.expression() == expression


def test_context_can_be_created():
    context = _numathap.Context()

    assert context is not None


def test_context_can_store_variable():
    context = _numathap.Context()

    context.set_value("x", "3")

    assert context.has_value("x")
    assert context.find_value("x") == "3"


def test_context_returns_none_for_undefined_variable():
    context = _numathap.Context()

    assert not context.has_value("x")
    assert context.find_value("x") is None


def test_context_can_clear_values():
    context = _numathap.Context()
    context.set_value("x", "3")

    context.clear()

    assert not context.has_value("x")
    assert context.find_value("x") is None
