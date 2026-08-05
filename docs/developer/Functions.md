# Functions and Constants

## Overview

This document describes the mathematical functions and constants currently recognized by **numathap**.

Function and constant resolution is performed by the selected **Math Adapter**. The default adapter is based on the C++ Standard Library (`cmath`) and currently provides the functionality documented below.

Function names and constant names are **case-sensitive**.

---

# Constants

The following mathematical constants are available.

| Constant     | Description              |
| ------------ | ------------------------ |
| `pi`         | Archimedes' constant (π) |
| `e`          | Euler's number           |
| `phi`        | Golden ratio (φ)         |
| `sqrt2`      | Square root of 2         |
| `sqrt3`      | Square root of 3         |
| `inv_pi`     | Reciprocal of π (1/π)    |
| `inv_sqrtpi` | Reciprocal of √π (1/√π)  |
| `ln2`        | Natural logarithm of 2   |
| `ln10`       | Natural logarithm of 10  |
| `log2e`      | Base-2 logarithm of *e*  |
| `log10e`     | Base-10 logarithm of *e* |

Example:

```cpp
auto ast = prepare("2 * pi");
```

---

# Trigonometric Functions

| Function      | Arguments | Description                       |
| ------------- | --------: | --------------------------------- |
| `sin(x)`      |         1 | Sine                              |
| `cos(x)`      |         1 | Cosine                            |
| `tan(x)`      |         1 | Tangent                           |
| `asin(x)`     |         1 | Arc sine                          |
| `acos(x)`     |         1 | Arc cosine                        |
| `atan(x)`     |         1 | Arc tangent                       |
| `atan2(y, x)` |         2 | Arc tangent using two coordinates |

Example:

```cpp
auto ast = prepare("sin(pi / 4)");
```

---

# Hyperbolic Functions

| Function  | Arguments | Description        |
| --------- | --------: | ------------------ |
| `sinh(x)` |         1 | Hyperbolic sine    |
| `cosh(x)` |         1 | Hyperbolic cosine  |
| `tanh(x)` |         1 | Hyperbolic tangent |

Example:

```cpp
auto ast = prepare("cosh(x)");
```

---

# Exponential and Logarithmic Functions

| Function   | Arguments | Description                 |
| ---------- | --------: | --------------------------- |
| `exp(x)`   |         1 | Exponential function (*e*ˣ) |
| `log(x)`   |         1 | Natural logarithm           |
| `log10(x)` |         1 | Base-10 logarithm           |
| `log2(x)`  | 1         | Base-2 logarithm            |

Example:

```cpp
auto ast = prepare("exp(log(x))");
```

---

# Power Functions

| Function    | Arguments | Description                 |
| ----------- | --------: | --------------------------- |
| `sqrt(x)`   |         1 | Square root                 |
| `cbrt(x)`   |         1 | Cube root                   |
| `pow(x, y)` |         2 | Raises *x* to the power *y* |
| `hypot(x, y)` | 2       | Euclidean norm: √(x² + y²)  |

Example:

```cpp
auto ast = prepare("pow(x, 3)");
```

---

# Special Mathematical Functions

The following functions provide commonly used special mathematical operations.

| Function | Arguments | Description |
|----------|----------:|-------------|
| `erf(x)` | 1 | Error function |
| `erfc(x)` | 1 | Complementary error function |
| `tgamma(x)` | 1 | Gamma function Γ(x) |
| `lgamma(x)` | 1 | Natural logarithm of the gamma function |

Example:

```cpp
auto ast = prepare("tgamma(x)");
```

---

# Numeric Functions

| Function   | Arguments | Description                          |
| ---------- | --------: | ------------------------------------ |
| `abs(x)`   |         1 | Absolute value                       |
| `floor(x)` |         1 | Largest integer not greater than *x* |
| `ceil(x)`  |         1 | Smallest integer not less than *x*   |
| `round(x)` |         1 | Rounds to the nearest integer        |

Example:

```cpp
auto ast = prepare("round(x)");
```

---

# Remainder Functions

These functions perform floating-point remainder operations.

| Function | Arguments | Description |
|----------|----------:|-------------|
| `fmod(x, y)` | 2 | Floating-point remainder of x divided by y |

Example:

```cpp
auto ast = prepare("fmod(x, y)");
```

---

# Argument Validation

Every function validates its number of arguments before execution.

For example,

```text
pow(x)
```

produces an exception because `pow()` requires exactly two arguments.

Likewise,

```text
sin(x, y)
```

produces an exception because `sin()` accepts exactly one argument.

---

# Unsupported Functions

If a function is not provided by the selected Math Adapter, numathap throws an exception.

Example:

```text
gamma(x)
```

produces an error similar to

```text
Function 'gamma' is not available in the selected math library.
```

The exact set of available functions depends on the selected Math Adapter.

---

# Unsupported Constants

Likewise, referencing an unknown constant produces an exception.

Example:

```text
tau
```

produces an error similar to

```text
Constant 'tau' is not available in the selected math library.
```

---

# Notes

* Function names are case-sensitive.
* Constant names are case-sensitive.
* Argument validation is performed before function execution.
* The current implementation uses the `cmath` Math Adapter.
* The available functions correspond to the operations explicitly mapped by the selected Math Adapter.
* Additional Math Adapters may provide different sets of functions and constants in future versions.