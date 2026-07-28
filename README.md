<!--
[![](https://img.shields.io/badge/buy_me_a_coffee-FFDD00?style=for-the-badge&logo=buy-me-a-coffee&logoColor=black)](https://antonio)
-->

![](https://img.shields.io/badge/C++-00599C?style=flat-square&logo=c%2B%2B&logoColor=white)

## Contents

- [Features](#features)
- [Example](#example)
- [Purpose](#purpose)
- [Architecture](#architecture)
- [Algorithms](#algorithms)
- [Current Status](#current-status)
- [Documentation](#documentation)
- [Project Philosophy](#project-philosophy)
- [Contributing](#contributing)
- [License](#license)

# Numathap

**Numathap** is a modern open-source numerical computing library written in C++, focused on numerical integration, limit computation, and scientific computing. The project combines high-performance native code with optional Python bindings, providing a flexible environment for both production applications and scientific prototyping.

The library is designed around a modular architecture that separates expression parsing, symbolic transformations, and numerical evaluation, making it straightforward to extend with new algorithms while preserving a clean and stable public API.

![Build](https://github.com/acarlos01sc/numathap/actions/workflows/ci.yml/badge.svg)
![Documentation](https://img.shields.io/badge/docs-online-brightgreen)
![License](https://img.shields.io/github/license/acarlos01sc/numathap)
![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)

---

# Features

- Modern C++ implementation
- Numerical expression parser
- Efficient expression evaluation
- Adaptive Simpson quadrature
- Python bindings
- Modular and extensible architecture
- Focus on numerical stability and correctness
- Clean and maintainable code
- Doxygen documentation
- Designed for scientific reproducibility

---

# Example

```cpp
#include <numathap/numathap.hpp>

using namespace numathap;

auto expr = prepare("sin(x)");

Context ctx;
ctx.setInterval("x", "0", "pi");

Value result = integrate(expr, "x", ctx);

std::cout << result << std::endl;
```

---

# Purpose

The primary goal of Numathap is to provide a reliable, modern, and extensible toolkit for numerical computation.

The project aims to support scientific and engineering applications by offering robust implementations of classical numerical algorithms while maintaining a clean software architecture that encourages future growth.

Although performance is an important objective, equal emphasis is placed on:

- Numerical correctness
- Reproducibility of results
- Code readability
- Long-term maintainability

---

# Architecture

Numathap is organized into independent components that separate mathematical representation from algorithm implementation.

Its architecture follows a pipeline similar to:

```text
Expression
    │
    ▼
Parser
    │
    ▼
Math AST
    │
    ▼
Prepared AST
    │
    ├── Numerical backends
    └── Symbolic backends
```

This separation allows new numerical or symbolic algorithms to be introduced without affecting the public API or existing components.

---

# Algorithms

Numathap implements classical algorithms described in the numerical analysis literature.

### Current implementation

- Adaptive Simpson quadrature

### Planned algorithms

- Gauss-Legendre quadrature
- Gauss-Kronrod quadrature
- Clenshaw-Curtis quadrature
- Romberg integration
- Additional numerical methods

The project focuses on careful implementation, numerical robustness, and clear documentation rather than introducing proprietary mathematical techniques.

---

# Current Status

Numathap is under active development.

### Implemented

- Expression parser
- Abstract syntax tree generation
- Expression evaluation
- Adaptive Simpson integration
- Python bindings
- Configurable numerical environment

### Planned

- Additional quadrature methods
- Symbolic differentiation
- Algebraic simplification
- Additional mathematical utilities

---

# Python Bindings

Python bindings allow Numathap to be integrated into scientific workflows, notebooks, and data analysis pipelines while preserving the computational performance of the C++ implementation.

---

# Documentation

The project documentation is automatically generated using Doxygen and published through GitHub Pages.

It includes:

- API reference
- Architecture overview
- Algorithm descriptions
- Implementation notes

---

# Project Philosophy

Numathap is guided by a few fundamental principles:

- Scientific correctness
- Transparency of implemented algorithms
- Reproducibility of numerical results
- Clean and maintainable code
- Modular software architecture
- Long-term extensibility

Rather than becoming a collection of isolated numerical routines, Numathap aims to provide a coherent framework where new algorithms can be incorporated naturally as the library evolves.

---

# Use of Artificial Intelligence Tools

Artificial Intelligence tools have been used throughout the development process as engineering assistants.

These tools have supported activities such as:

- Code review
- Implementation refinement
- Algorithm exploration
- Documentation drafting
- Conceptual discussions

All architectural decisions, implementations, validations, and final technical choices remain the responsibility of the project authors.

---

# Contributing

Contributions, suggestions, bug reports, and discussions are always welcome.

Whether you are interested in numerical analysis, modern C++, scientific computing, or documentation, your participation is appreciated.

---

# License

Numathap is released under an open-source license.

See the `LICENSE` file for additional information.