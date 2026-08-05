# numathap Python User Guide

## Overview

`numathap` is a numerical mathematics library that provides a Python API
built on top of a modern C++ computational core.

The library is designed for numerical expression processing and provides
support for:

* mathematical expression parsing;
* expression preparation;
* numerical evaluation;
* numerical integration;
* configurable mathematical environments.

The Python package uses the same computational engine available through the
C++ API, combining Python usability with the performance of a native C++
implementation.

---

# Installation

The package can be installed using `pip`:

```bash
pip install numathap
```

After installation, the library can be imported normally:

```python
import numathap

print(numathap.__version__)
```

---

# Basic Concepts

The main workflow in `numathap` is:

```
Expression
     |
     v
prepare()
     |
     v
Prepared Expression
     |
     +----------------+
     |                |
     v                v
evaluate()       integrate()
```

Expressions are provided as strings and are converted into an internal
representation by `prepare()`.

The prepared expression can then be evaluated or used by numerical algorithms.

---

# Preparing Expressions

The `prepare()` function converts a mathematical expression string into a
prepared expression object.

Example:

```python
import numathap

expr = numathap.prepare("sin(x) + x^2")
```

The resulting object stores a prepared representation of the mathematical expression and can be reused for multiple operations.

Preparing an expression once and reusing it can avoid unnecessary parsing
operations.

---

# Evaluating Expressions

Prepared expressions can be evaluated using `evaluate()`.

Example:

```python
import numathap

expr = numathap.prepare("sin(x)")
ctx = numathap.Context()
ctx.set_value("x","pi")

result = numathap.evaluate(
    expr,
    ctx
)

print(result)
```

The evaluation process uses the configured mathematical environment and
numeric backend.

---

# Numerical Integration

`numathap` provides numerical integration through the `integrate()` function.

Example:

```python
import numathap

expr = numathap.prepare("sin(x)")
ctx = numathap.Context()
ctx.set_interval("x","0","pi")

result = numathap.integrate(
    expr,
    "x",
    ctx
)

print(result)
```

The integration subsystem supports configurable numerical algorithms and can
be extended with additional integration methods.

---

# Mathematical Environment

The behavior of the library can be configured through `MathEnvironment`.

Example:

```python
import numathap

env = numathap.MathEnvironment()

expr = numathap.prepare(
    "sin(x)",
    env
)
```

The mathematical environment controls library configuration, including:

* mathematical functions and constants;
* numeric representation;
* backend capabilities;
* algorithm configuration.

The default environment is suitable for most common use cases.

---

# Configuration

The `configure()` function provides a convenient way to configure a
`MathEnvironment`.

Example:

```python
import numathap

env = numathap.configure(
    numathap.Capability.Simplify
)
```

Additional configuration options will be introduced as new numerical capabilities are added to the library.

---

# Reusing Prepared Expressions

Prepared expressions can be reused for multiple evaluations.

Example:

```python
import numathap

expr = numathap.prepare("x^2 + sin(x)")
ctx = numathap.Context()

ctx.set_value("x","1")
value1 = numathap.evaluate(
    expr,
    ctx
)

ctx.set_value("x","2")
value2 = numathap.evaluate(
    expr,
    ctx
)

print(value1)
print(value2)
```

This is useful when the same expression needs to be evaluated many times.

---

# Version Information

The installed package version is available through `__version__`.

Example:

```python
import numathap

print(numathap.__version__)
```

---

# C++ API

The Python package is built on top of the `numathap` C++ library.

For C++ installation and usage, see:

```
docs/cpp_user.md
```

The C++ API exposes the same computational engine and is intended for applications requiring native C++ integration.

---

# License

`numathap` is distributed under the MIT License.
