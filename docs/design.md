# Overview

**numathap** is designed to provide a simple and intuitive interface for numerical mathematics. Users write mathematical expressions as strings, and the library transforms them into an internal representation that can be efficiently evaluated by different numerical algorithms.

Much like a compiler, the input text is first tokenized and then parsed according to the rules of mathematical precedence and associativity. The result is an **Abstract Syntax Tree (AST)**, which provides a structured semantic representation of the expression and serves as the foundation for subsequent numerical computations.

# Processing Pipeline

A numerical computation typically consists of the following stages:

- **prepare()** – Parses a mathematical expression and builds its Abstract Syntax Tree (AST).
- **Context** – Defines the numerical context, including variable values and integration intervals.
- **Computation** – Executes the desired numerical operation. Currently, the library provides:
  - **evaluate()** – Evaluates an expression.
  - **integrate()** – Computes a definite integral.

# Simple Examples

## C++

```cpp
#include <iostream>
#include <numathap/numathap.hpp>

int main()
{
    using namespace numathap;

    auto expr = prepare("sin(x)");

    Context ctx;
    ctx.setValue("x", "pi/2");

    auto value = evaluate(expr, ctx);
    std::cout << "Evaluation: " << value << '\n';

    ctx.setInterval("x", "0", "pi/2");

    auto area = integrate(expr, "x", ctx);
    std::cout << "Integral: " << area << '\n';

    return 0;
}
```

## Python

```python
import numathap as np

expr = np.prepare("sin(x)")

ctx = np.Context()
ctx.set_value("x", "pi/2")

print("Evaluation:", np.evaluate(expr, ctx))

ctx.set_interval("x", "0", "pi/2")

print("Integral:", np.integrate(expr, "x", ctx))
```