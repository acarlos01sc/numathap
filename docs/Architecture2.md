# Numathap Architecture Overview

## Overview

The **numathap** library provides a high-level API for mathematical and numerical computation in **C++** and provides an interface to **Python**.

Its primary goal is to allow mathematical expressions to be represented as strings and processed by multiple services such as numerical evaluation, integration, limit computation, symbolic differentiation, algebraic simplification, optimization, and future mathematical capabilities.

The architecture is inspired by classical compiler design while introducing a clear separation between:

- syntactic representation;
- mathematical representation;
- execution environment;
- execution strategy.

This separation allows multiple algorithms and services to operate on the same mathematical model without coupling expression representation to any particular mathematical library.

---

# Public API

The primary user entry point is the **prepare** function and **Context**.

The default configuration uses the C++ Standard Library (`cmath`).

Advanced users may customize the environment by selecting another supported mathematical library or enabling additional capabilities.

Example:

```cpp
numathap::Context ctx;
auto prep = numathap::prepare("sin(x)+sqrt(x)");
ctx.setValue("x","pi");
auto result = numathap::evaluate(prep);
```

---

