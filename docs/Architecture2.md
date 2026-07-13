# Numathap Architecture Overview

## Overview

**numathap** is a mathematical computation library for **C++** with Python bindings designed around a layered architecture inspired by classical compiler design.

Rather than evaluating expressions directly from text, numathap transforms an expression through a sequence of progressively richer representations until it reaches a canonical intermediate representation called the **Prepared-AST**.

This representation is then shared by multiple execution backends, allowing numerical and symbolic algorithms to operate on the same mathematical model without repeating parsing or preparation.

The architecture intentionally separates:

- syntactic analysis;
- mathematical semantics;
- expression preparation;
- execution algorithms;
- mathematical library integration.

This separation provides three important benefits:

- expressions are parsed only once;
- preparation is performed only once;
- multiple numerical and symbolic algorithms can reuse the same prepared expression.

The overall architecture resembles a compiler pipeline while remaining specialized for mathematical computation.

---

# Design Philosophy

The architecture of numathap follows a few fundamental principles.

## Separation of Responsibilities

Each component has a single responsibility.

Examples include:

- the Lexer recognizes lexical elements;
- the Parser validates grammar;
- the Math-AST Builder assigns mathematical semantics;
- the Orchestrator prepares expressions for execution;
- numerical and symbolic backends execute mathematical algorithms.

No component performs responsibilities belonging to another stage.

---

## Canonical Intermediate Representation

The **Prepared-AST** is the canonical representation of a mathematical expression inside numathap.

Every execution backend operates on this representation instead of directly processing source text.

Once a Prepared-AST has been created, it can be reused by any backend without rebuilding the expression.

---

## Backend Independence

Execution backends are completely independent of the parser implementation.

They consume only Prepared-AST objects.

Consequently, future expression frontends may generate Math-AST objects without affecting numerical or symbolic algorithms.

---

## Mathematical Library Independence

Numerical components never invoke external mathematical libraries directly.

Instead, all mathematical functions and constants are resolved through the **MathAdapter** abstraction.

This allows different mathematical libraries to be selected without modifying numerical algorithms.

---

## Extensibility

The architecture has been designed so that new functionality can be added by introducing new components rather than modifying existing ones.

Examples include:

- new mathematical libraries;
- new symbolic algorithms;
- new numerical algorithms;
- new optimization capabilities;
- new expression preparation stages.

---

# Public API

The primary user entry point is **Session**.

A Session represents an isolated mathematical execution environment.

Each Session owns:

- a Configurator;
- a Math Environment;
- execution services.

Most users interact exclusively with Session.

Example:

```cpp
numathap::Session session;

auto value =
    session.evaluate("sin(x) + sqrt(x)");
```

Advanced users may customize the execution environment before invoking mathematical services.

Example:

```cpp
numathap::Session session;

auto& config = session.configurator();

config.setMathLibrary(
    MathLibrary::Boost
);
```

Future versions may expose additional services through the same Session object, including symbolic and numerical algorithms.

---

# Configuration System

The execution environment is configured through two cooperating components:

- Configurator
- Math Environment

This separation keeps configuration independent from execution.

---

## Configurator

The Configurator is the public interface responsible for configuring a Session.

It allows users to select:

- mathematical library;
- numeric type;
- optional capabilities;
- future execution options.

The Configurator performs no mathematical computation.

Its responsibility is limited to assembling a valid execution environment.

---

## Math Environment

The Math Environment represents the complete runtime configuration associated with a Session.

It stores:

- selected mathematical library;
- numeric type;
- enabled capabilities;
- Math Adapter instance.

Execution components receive the Math Environment as read-only configuration.

The Math Environment performs no parsing and no mathematical computation.

Its sole purpose is to provide runtime information required by execution backends.

---

# Mathematical Library Integration

## Math Adapter

The Math Adapter isolates numathap from external mathematical libraries.

Instead of directly invoking library-specific functions, numerical backends communicate exclusively through this interface.

The Math Adapter is responsible for:

- resolving mathematical functions;
- resolving mathematical constants;
- adapting library-specific behavior;
- presenting a uniform interface to execution components.

Consequently, the Evaluator and all future numerical backends remain completely independent of any particular mathematical library.

---

## Supported Mathematical Libraries

The initial release of numathap supports:

- C++ Standard Library (`cmath`);
- Boost.Math.

Future versions may support:

- arbitrary-precision libraries;
- interval arithmetic libraries;
- user-defined mathematical libraries;
- domain-specific numerical engines.

Each library is encapsulated by its own Math Adapter implementation.

Adding support for a new library therefore requires implementing only a new adapter, without modifying the parser, mathematical representations or execution algorithms.

---

# Architectural Layers

The overall architecture may be viewed as a sequence of independent layers.

```text
User
 │
 ▼
Session
 │
 ▼
Configuration
 │
 ▼
Expression Analysis
 │
 ▼
Mathematical Representation
 │
 ▼
Expression Preparation
 │
 ▼
Execution Backends
```

Each layer communicates only with adjacent layers.

This organization minimizes coupling while allowing future components to be introduced without affecting existing implementations.

---

# Expression Processing

## Overview

The first stage of the execution pipeline transforms a textual mathematical
expression into a structured mathematical representation.

Rather than directly evaluating source text, numathap progressively enriches
the information associated with an expression through three independent
representations:

```text
Input String
      │
      ▼
    Lexer
      │
      ▼
    Parser
      │
      ▼
 Parser-AST
      │
      ▼
Math-AST Builder
      │
      ▼
   Math-AST
      │
      ▼
 Orchestrator
      │
      ▼
 Prepared-AST
```

Each stage has a single well-defined responsibility and is independent of
subsequent execution algorithms.

---

# Token

A **Token** represents the smallest lexical element recognized by the language.

Examples include:

- numeric literals;
- identifiers;
- operators;
- delimiters;
- punctuation symbols.

Examples:

```text
NUMBER
IDENTIFIER
PLUS
MINUS
POWER
LEFT_PAREN
RIGHT_PAREN
COMMA
```

Tokens contain no grammatical or mathematical meaning.

Their sole purpose is to represent the lexical structure of the source
expression.

---

# Lexer

The **Lexer** converts a character stream into an ordered sequence of tokens.

For example,

```text
sin(x)+2*x
```

becomes

```text
FUNCTION(sin)
LEFT_PAREN
IDENTIFIER(x)
RIGHT_PAREN
PLUS
NUMBER(2)
MULTIPLY
IDENTIFIER(x)
```

The Lexer performs no grammatical validation and contains no mathematical
knowledge.

Its responsibility ends after lexical analysis.

---

# Parser

The **Parser** validates the grammatical structure of the token stream.

If the expression is syntactically correct, it produces the **Parser-AST**.

If the expression violates the grammar, an exception describing the parsing
error is generated.

The Parser is intentionally unaware of mathematical semantics.

For example,

```text
f(x)
```

is parsed as a generic function call.

The Parser does not determine whether the function actually exists.

Likewise,

```text
pi
```

is parsed as an identifier.

The Parser does not determine whether the identifier represents a variable,
a constant or another mathematical object.

---

# Parser-AST

The Parser-AST is a purely syntactic representation.

Its nodes represent grammatical constructs rather than mathematical concepts.

Typical node kinds include:

- numeric literal;
- identifier;
- unary expression;
- binary expression;
- function call;
- parenthesized expression.

The Parser-AST preserves the exact grammatical organization produced by the
Parser.

It intentionally contains no semantic information.

---

# Math-AST Builder

The **Math-AST Builder** converts the Parser-AST into a mathematical
representation.

This transformation separates syntax from mathematical meaning.

Equivalent syntactic constructs may therefore produce identical mathematical
representations.

The Math-AST Builder is also the architectural boundary between parsing and
mathematical processing.

Future frontends may construct Math-AST objects without using the current
Parser implementation.

---

# Math-AST

The Math-AST represents the mathematical structure of an expression.

Unlike the Parser-AST, it no longer reflects grammatical details.

Instead, it models mathematical concepts.

Typical node kinds include:

- NumberNode;
- SymbolNode;
- UnaryNode;
- BinaryNode;
- FunctionNode.

The Math-AST is independent of the parsing process and serves as the input to
the preparation stage.

---

# Orchestrator

The **Orchestrator** prepares a Math-AST for execution.

It is responsible for applying transformations required by the capabilities
enabled in the current Math Environment.

The Orchestrator never performs numerical evaluation.

Likewise, it performs no symbolic computation beyond the preparation process.

Possible preparation stages include:

- constant folding;
- symbolic normalization;
- algebraic simplification;
- canonicalization;
- structural optimization;
- capability-specific transformations.

Current versions of numathap perform no transformations.

Consequently, the Math-AST and Prepared-AST are currently identical.

Nevertheless, the Orchestrator establishes the architectural boundary where
future preparation stages will be introduced.

---

# Prepared-AST

The **Prepared-AST** is the canonical intermediate representation used by all
execution backends.

Once a Prepared-AST has been produced, no further structural modifications are
performed during numerical evaluation.

This allows expensive preparation stages to be executed only once.

The same Prepared-AST may subsequently be reused by multiple execution
algorithms.

Typical examples include:

- repeated numerical evaluation;
- numerical integration;
- limit computation;
- root finding;
- optimization;
- symbolic differentiation;
- algebraic simplification.

The Prepared-AST is therefore designed to be treated as an immutable object
that can safely be shared among independent execution backends.

It represents the principal communication interface between expression
preparation and mathematical execution.

---

# Preparation Pipeline

The complete preparation stage may be summarized as follows:

```text
Input String
      │
      ▼
    Lexer
      │
      ▼
    Parser
      │
      ▼
 Parser-AST
      │
      ▼
Math-AST Builder
      │
      ▼
   Math-AST
      │
      ▼
 Orchestrator
      │
      ▼
 Prepared-AST
```

All subsequent numerical and symbolic processing begins from the
Prepared-AST.

---

# Shared Execution Infrastructure

## Overview

Once an expression has been transformed into a **Prepared-AST**, it becomes
independent of the parsing and preparation stages.

Execution is performed by a collection of reusable backends that operate
directly on the Prepared-AST.

All execution backends share a small set of common infrastructure:

- Dispatcher;
- Context;
- Value;
- Math Environment;
- Math Adapter.

This common infrastructure ensures that numerical and symbolic algorithms
operate on the same mathematical representation while remaining independent
from one another.

---

# Dispatcher

## Purpose

The **Dispatcher** centralizes runtime type dispatch for the Prepared-AST.

Rather than embedding behavior into MathNode classes, the Dispatcher identifies
the concrete node type and forwards it to the appropriate visitor.

This design intentionally keeps the AST free from execution logic.

Typical dispatch sequence:

```text
Prepared-AST
      │
      ▼
 Dispatcher
      │
      ▼
 Visitor
```

The Dispatcher performs:

- runtime type identification;
- visitor dispatch;
- no mathematical computation.

Consequently, new execution algorithms can be implemented without modifying
the AST hierarchy.

---

## Architectural Role

Every execution backend traverses the Prepared-AST through the Dispatcher.

Examples include:

- numerical evaluation;
- symbolic differentiation;
- algebraic simplification;
- symbolic expansion;
- future symbolic algorithms.

The Dispatcher therefore becomes the common entry point into the mathematical
representation.

---

# Context

## Purpose

The **Context** represents the runtime environment used during numerical
evaluation.

It stores information that may change from one evaluation to another without
requiring reconstruction of the Prepared-AST.

Typical examples include:

- variable values;
- user-defined symbols;
- future execution state.

Example:

```cpp
Context context;

context.set("x", 2.0);
context.set("y", 5.0);
```

---

## Reusability

Because the Prepared-AST is immutable, the same expression may be evaluated
under multiple Context objects.

Example:

```text
Prepared-AST
      │
      ├────────► Context(x = 1)
      │
      ├────────► Context(x = 2)
      │
      ├────────► Context(x = 3)
      │
      └────────► Context(x = 4)
```

This separation is fundamental for numerical algorithms requiring repeated
evaluation.

---

# Value

The **Value** class represents numerical results produced by execution
backends.

Rather than exposing implementation-specific numeric types, numathap uses
Value as a uniform abstraction.

Current implementations support real numbers.

Future versions may extend Value to support:

- arbitrary precision;
- complex numbers;
- interval arithmetic;
- automatic differentiation types;
- additional numeric representations.

Every numerical backend exchanges results through Value.

---

# Execution Backends

## Overview

Execution backends consume a Prepared-AST and perform a mathematical operation.

Different backends may produce different outputs while operating on the same
Prepared-AST.

Examples:

```text
Prepared-AST
      │
      ├────────► Evaluator
      │              │
      │              ▼
      │            Value
      │
      ├────────► Differentiator
      │              │
      │              ▼
      │         Derived Math-AST
      │
      ├────────► Simplifier
      │              │
      │              ▼
      │      Simplified Math-AST
      │
      └────────► Future Backends
```

This organization allows numerical and symbolic algorithms to evolve
independently.

---

# Evaluator

## Purpose

The **Evaluator** performs numerical evaluation of a Prepared-AST.

Unlike earlier stages of the processing pipeline, the Evaluator performs no
parsing and no expression preparation.

Instead, it repeatedly evaluates an already prepared mathematical expression.

---

## Two-Stage Operation

The Evaluator operates in two distinct phases.

### Preparation

A Prepared-AST is associated with the Evaluator.

```cpp
Evaluator evaluator(adapter);

evaluator.prepare(prepared_ast);
```

Preparation stores a reference to the immutable Prepared-AST.

No numerical computation occurs during this stage.

---

### Evaluation

Once prepared, the Evaluator may execute the expression repeatedly using
different Context objects.

Example:

```cpp
auto v1 = evaluator.calc(context1);

auto v2 = evaluator.calc(context2);

auto v3 = evaluator.calc(context3);
```

The Prepared-AST is never rebuilt.

Only variable values supplied through Context change between evaluations.

---

## Mathematical Library Independence

Whenever a mathematical function or constant must be resolved, the Evaluator
delegates the operation to the Math Adapter associated with the current Math
Environment.

The Evaluator therefore contains no knowledge of:

- cmath;
- Boost.Math;
- future mathematical libraries.

Library-specific behavior remains completely isolated inside Math Adapter
implementations.

---

# Numerical Backends

Numerical backends repeatedly evaluate a Prepared-AST under different runtime
contexts.

Typical examples include:

- numerical integration;
- limit computation;
- root finding;
- optimization;
- equation solving;
- future numerical algorithms.

Rather than parsing an expression themselves, these components prepare an
Evaluator once and reuse it throughout the execution of their algorithms.

For example, an Integrator may evaluate

```text
f(a)
f(b)
f(c)
...
```

using the same Prepared-AST while changing only the Context supplied to the
Evaluator.

This significantly reduces the computational cost of repeated evaluation.

---

# Symbolic Backends

Symbolic backends also consume the Prepared-AST.

Unlike numerical backends, however, they do not produce numerical values.

Instead, they generate new mathematical representations.

Typical symbolic backends include:

- Differentiator;
- Simplifier;
- Expander;
- Factorizer;
- future symbolic algorithms.

Example:

```text
Prepared-AST
      │
      ▼
Differentiator
      │
      ▼
Derivative Math-AST
```

The resulting mathematical tree may itself be submitted to the Orchestrator,
producing another Prepared-AST suitable for subsequent numerical or symbolic
processing.

This allows symbolic and numerical algorithms to cooperate while remaining
architecturally independent.

---

# Shared Backend Philosophy

All execution backends share the same design philosophy.

They:

- consume Prepared-AST objects;
- never perform parsing;
- never perform lexical analysis;
- never modify the original Prepared-AST;
- remain independent of external mathematical libraries.

By separating expression preparation from execution, numathap enables a single
Prepared-AST to be reused efficiently across a wide range of mathematical
algorithms.

---

# Mathematical Services

## Overview

Mathematical services are high-level algorithms built on top of the execution
backends.

Rather than parsing mathematical expressions themselves, these services operate
on a previously prepared mathematical representation.

This architecture separates expression preparation from algorithm execution,
allowing expensive preparation stages to be performed only once.

Typical workflow:

```text
Input String
      │
      ▼
Preparation Pipeline
      │
      ▼
 Prepared-AST
      │
      ├────────► Evaluator
      │
      ├────────► Integrator
      │
      ├────────► Limit Calculator
      │
      ├────────► Differentiator
      │
      └────────► Future Services
```

All services consume the same Prepared-AST.

---

# Integrator

The Integrator performs numerical integration over a mathematical expression.

Internally, it prepares an Evaluator once and repeatedly evaluates the same
Prepared-AST for different integration points.

Conceptually:

```text
Prepared-AST
      │
      ▼
 Evaluator.prepare()
      │
      ├────────► calc(Context(x₁))
      ├────────► calc(Context(x₂))
      ├────────► calc(Context(x₃))
      └────────► ...
```

Only the Context changes during the integration process.

The mathematical expression itself is never rebuilt.

This strategy greatly improves efficiency for adaptive numerical algorithms
such as Simpson's Rule or Gauss-Kronrod quadrature.

---

# Limit Calculator

The Limit Calculator estimates limits through successive numerical evaluations.

Like the Integrator, it prepares an Evaluator once and evaluates the same
Prepared-AST repeatedly.

Typical evaluation sequence:

```text
f(x+h)

f(x+h/2)

f(x+h/4)

f(x+h/8)

...
```

Each evaluation uses a different Context while sharing the same Prepared-AST.

---

# Root Finder

Future root-finding algorithms will follow the same execution model.

Examples include:

- Bisection;
- Newton-Raphson;
- Secant Method;
- Brent's Method.

Each algorithm repeatedly evaluates the same Prepared-AST while modifying only
the variable values contained in the Context.

---

# Optimizer

Optimization algorithms will likewise reuse the same Prepared-AST.

Examples include:

- gradient-based optimization;
- derivative-free optimization;
- constrained optimization;
- future optimization techniques.

Only runtime Context objects vary throughout the optimization process.

---

# Differentiator

The Differentiator is a symbolic backend.

Unlike numerical algorithms, it does not produce Value objects.

Instead, it transforms one mathematical representation into another.

Example:

```text
x² + sin(x)
```

becomes

```text
2*x + cos(x)
```

Conceptually:

```text
Prepared-AST
      │
      ▼
Differentiator
      │
      ▼
Derivative Math-AST
```

The resulting mathematical tree may subsequently be submitted to the
Orchestrator to produce another Prepared-AST.

This new Prepared-AST may then be evaluated numerically or processed by other
symbolic algorithms.

---

# Future Symbolic Services

Future symbolic services may include:

- simplification;
- expansion;
- factorization;
- symbolic substitution;
- algebraic normalization;
- symbolic optimization.

All symbolic algorithms operate on Prepared-AST objects and generate new
mathematical representations.

---

# Processing Pipeline

```text
                             User
                               |
                               v
                           Session
                               |
                               v
                        Configurator
                               |
                               v
                      Math Environment
                               |
               +---------------+---------------+
               |                               |
               |                         Math Adapter
               |                               |
               |               +---------------+---------------+
               |               |                               |
               |               v                               v
               |             cmath                       Boost.Math
               |
Input String   |
     |         |
     v         |
   Lexer       |
     |         |
     v         |
   Parser      |
     |         |
     v         |
 Parser-AST    |
     |         |
     v         |
Math-AST Builder
     |
     v
  Math-AST
     |
     v
 Orchestrator
     |
     v
 Prepared-AST
     |
     +---------------------------+
     |                           |
     v                           v
Dispatcher                Symbolic Backends
     |
     v
 Evaluator
     |
     +---------------+
     |               |
     v               v
 Context          Math Adapter
     |
     v
  Value
```

The Dispatcher provides the common traversal mechanism used by execution
backends.

The Prepared-AST represents the architectural boundary between expression
preparation and mathematical execution.

---

# Architecture Summary

## User Layer

- Session

---

## Configuration Layer

- Configurator
- Math Environment
- Math Adapter

---

## Expression Analysis

- Token
- Lexer
- Parser
- Parser-AST

---

## Mathematical Representation

- Math-AST
- Math-AST Builder

---

## Preparation

- Orchestrator
- Prepared-AST

---

## Shared Infrastructure

- Dispatcher
- Context
- Value

---

## Execution Backends

### Numerical

- Evaluator
- Integrator
- Limit Calculator
- Root Finder
- Optimizer
- Future numerical algorithms

### Symbolic

- Differentiator
- Simplifier
- Expander
- Factorizer
- Future symbolic algorithms

---

# Design Principles

The architecture of numathap is based on the following principles.

## Separation of Concerns

Each stage performs exactly one responsibility.

Parsing, mathematical representation, preparation and execution remain
independent.

---

## Immutable Intermediate Representation

The Prepared-AST is treated as an immutable representation.

Execution algorithms never modify it.

---

## Expression Reuse

Expressions are parsed once.

Expressions are prepared once.

Execution algorithms may evaluate the same Prepared-AST thousands of times
under different Context objects.

---

## Backend Independence

Numerical and symbolic backends operate on the same Prepared-AST while
remaining completely independent of one another.

---

## Mathematical Library Independence

All mathematical functions and constants are resolved exclusively through the
Math Adapter abstraction.

Consequently, execution algorithms never depend directly on external
mathematical libraries.

---

## Extensibility

New mathematical libraries, symbolic algorithms and numerical methods can be
introduced without modifying the parser or the mathematical representation.

---

# Future Extensions

The current architecture intentionally leaves room for future developments,
including:

- arbitrary precision arithmetic;
- complex number support;
- interval arithmetic;
- tensor and matrix types;
- symbolic optimization;
- symbolic integration;
- automatic differentiation;
- user-defined mathematical libraries;
- user-defined execution backends.

These extensions can be incorporated while preserving the architectural
principles established by the current design.

---

# Conclusion

The architecture of **numathap** separates expression analysis, mathematical
representation, preparation and execution into independent layers.

The **Prepared-AST** serves as the canonical intermediate representation shared
by all execution backends.

This design enables efficient expression reuse, clean separation of
responsibilities and complete independence from external mathematical
libraries, providing a scalable foundation for both numerical and symbolic
computation.
