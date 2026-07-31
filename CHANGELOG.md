# Changelog

## Release v0.1.4

    - Algorithm Gauss-Kronrod 15 (GK15) implemented for `integrate`.
    - Improved `configure` to allow fine-grained GK15 configuration from Python.
    - Improved `configure` to allow fine-grained Adaptive Simpson configuration from Python.
    - Exposed `Value` comparison operators to Python.
    - Added symbolic differentiation of prepared mathematical expressions with respect to a specified variable.
    - Added support for differentiation rules including arithmetic operators, mathematical functions, the chain rule, and general power expressions.
    - Added the high-level C++ differentiate() API returning a simplified PreparedAst.
    - Exposed symbolic differentiation through the Python bindings as numathap.differentiate().
    - Added tests covering power differentiation, the chain rule, product and quotient rules, constants, and error handling.


## Release v0.1.3

    - Function `integrate` using Adaptive Simpson implemented.
    - Function `evaluate` implemented.