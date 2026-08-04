# Changelog

## Release v0.1.5

- Improved the symbolic simplifier by recognizing the identity `x - x = 0`.
- Added comprehensive Python type stubs (`__init__.pyi`) for the public API.
- Implemented factorial function using ! symbol.
- Implemented Constant Folder and inserted in differentiate pipeline.
- Included x^1 to x in Simplifier.
- Improved the binding of the configure function.


## Release v0.1.4

- Implemented the Gauss-Kronrod 15 (GK15) algorithm for `integrate`.
- Extended `configure` to support fine-grained GK15 configuration from Python.
- Extended `configure` to support fine-grained Adaptive Simpson configuration from Python.
- Exposed `Value` comparison operators to Python.
- Added symbolic differentiation for prepared mathematical expressions with respect to a specified variable.
- Added support for differentiation rules including arithmetic operators, mathematical functions, the chain rule, and general power expressions.
- Added the high-level C++ `differentiate()` API returning a simplified `PreparedAst`.
- Exposed symbolic differentiation to Python as `numathap.differentiate()`.
- Added tests covering power differentiation, the chain rule, product and quotient rules, constants, and error handling.
- Added IntelliSense support for the Python bindings.


## Release v0.1.3

- Function `integrate` using Adaptive Simpson implemented.
- Function `evaluate` implemented.