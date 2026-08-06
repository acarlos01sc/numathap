# Introduction

**numathap** is designed to provide a simple and intuitive interface for numerical mathematics. Users write mathematical expressions as strings, and the library transforms them into an internal representation that can be efficiently evaluated by different numerical algorithms.

Much like a compiler, the input text is first tokenized and then parsed according to the rules of mathematical precedence and associativity. The result is an **Abstract Syntax Tree (AST)**, which provides a structured semantic representation of the expression and serves as the foundation for subsequent numerical computations.

# Processing Pipeline

A numerical computation typically consists of the following stages:

- **prepare()** – Parses a mathematical expression and builds its Abstract Syntax Tree (AST).
- **Context** – Defines the numerical context, including variable values and integration intervals.
- **Computation** – Executes the desired numerical operation. Currently, the library provides:
  - **evaluate()** – Evaluates an expression.
  - **integrate()** – Computes a definite integral.
  - **differentiate()** - Builds a symbolic derivative AST. Default algorithm is Adaptive Simpson.
  - **series()** - Builds a symbolic series AST. The default is Taylor series type.

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

    auto expr_diff = differentiate(expr,"x");
    ctx.setValue("x","pi/2");
    auto dif = evaluate(expr_diff,ctx);
    std::cout << "Differentiate at point pi/2: " << dif << '\n';

    // Calculating via Taylor series
    auto taylor = series(expr,"x","pi/2");
    auto taylor_v = evaluate(taylor,ctx);
    std::cout << "Calculus via Taylor series: " << taylor_v << 'n';

    return 0;
}
```

## Python

```python
import numathap as np

expr = np.prepare("sin(x)")

ctx = np.Context()
ctx.set_value("x", "pi/2")

print("Evaluation: ", np.evaluate(expr, ctx))

ctx.set_interval("x", "0", "pi/2")

print("Integral: ", np.integrate(expr, "x", ctx))

expr_diff = np.differentiate(expr,"x")
ctx.set_value("x","pi/2")
print("Differentiate at point pi/2: ",np.evaluate(expr_diff,ctx))

taylor = np.series(expr,"x","pi/2")
print("Calculus via Taylor series: ",np.evaluate(taylor,ctx))

```