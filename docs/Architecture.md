# Numathap Architecture Overview

## Overview

The **numathap** library provides a high-level API for mathematical and
numerical computation in **C++** and **Python**.

Its primary goal is to allow mathematical expressions to be represented
as strings and processed by multiple services such as numerical
evaluation, integration, limit computation, symbolic differentiation,
algebraic simplification, optimization, and future mathematical
capabilities.

The architecture is inspired by classical compiler design while
introducing a clear distinction between:

* syntactic representation;
* mathematical representation;
* execution strategy.

This separation allows multiple algorithms and services to operate on
the same mathematical model without coupling expression representation
to any specific computation.

---

# Public API

The main user entry point is the **Session**.

A Session represents an isolated execution environment containing:

* mathematical library configuration;
* available capabilities;
* registered functions;
* execution services.

The user normally does not create internal components such as
Orchestrator, Dispatcher or FunctionRegistry directly.

Example:

```cpp
numathap::Session session;

auto result =
    session.evaluate("sin(x)+sqrt(x)");
```

Advanced users may customize the Session environment:

```cpp
session.setMathLibrary("custom_library");

session.enableCapability(
    Capability::Simplifier
);
```

The default configuration uses the standard mathematical library based
on `cmath`.

---

# Expression Analysis

## Token

A **Token** represents a lexical element of the mathematical language,
including numbers, identifiers, operators, delimiters and special
symbols.

---

## Lexer

The **Lexer** converts the input character stream into an ordered
sequence of tokens.

Example:

```text
sin(x) + 2*x
```

becomes:

```text
FUNCTION(sin)
LPAREN
IDENTIFIER(x)
RPAREN
PLUS
NUMBER(2)
MULTIPLY
IDENTIFIER(x)
```

---

## Parser

The **Parser** validates the grammar and builds the **Parser-AST**.

Parser nodes represent only syntactic structures:

* numeric literals;
* identifiers;
* unary operators;
* binary operators;
* function calls;
* other grammar elements.

Parser nodes contain no evaluation or mathematical behavior.

Example:

```text
sin(x) + x^2
```

```text
        +
       / \
    sin   ^
     |   / \
     x  x   2
```

---

# Parser-AST

The Parser-AST represents only the grammatical organization of the
expression.

It knows nothing about:

* numerical evaluation;
* symbolic manipulation;
* optimization;
* mathematical semantics.

Its purpose is to preserve the structure produced by parsing.

---

# Math-AST Builder

The **Math-AST Builder** converts the Parser-AST into a **Math-AST**.

While the Parser-AST models syntax, the Math-AST models mathematical
meaning.

Equivalent syntactic constructions may therefore produce the same
mathematical representation.

The Math-AST is independent of the parser implementation and may
eventually be produced by other frontends.

---

# Configuration System

The configuration system defines how a Session is assembled.

It separates user configuration from internal execution components.

## Configurator

The **Configurator** creates and modifies the mathematical execution
environment.

It controls:

* selected mathematical library;
* available capabilities;
* precision options;
* backend configuration.

The Configurator does not execute mathematical operations.

Its responsibility is to produce a configured environment.

---

## Math Environment

The **Math Environment** represents the resolved configuration used
during execution.

It contains:

* Function Registry;
* capability set;
* execution options;
* backend configuration.

Runtime components receive the environment instead of directly
consulting the Configurator.

The Math Environment does not implement mathematical functions.
It provides the services required by execution components.

---

# Orchestrator

The **Orchestrator** prepares a Math-AST using the capabilities enabled
by the current Math Environment.

It does not manage configuration.

Possible preparation steps include:

* simplification;
* normalization;
* algebraic rewriting;
* removal of redundant expressions;
* structural optimizations.

The result is the Prepared-AST.

---

# Shared Infrastructure

## Dispatcher

The **Dispatcher** routes operations to the correct backend
implementation without embedding behavior into node classes.

It allows different algorithms to operate on the same Prepared-AST.

---

## Context

The **Context** stores external information required during execution,
including:

* variable values;
* constants;
* user-defined symbols.

---

## Value

The **Value** type abstracts numeric objects manipulated by execution
backends.

Future versions may support:

* arbitrary precision;
* complex numbers;
* additional numeric representations.

---

## Function Registry

The **Function Registry** maps textual function names to mathematical
implementations provided by the selected mathematical library.

The Math-AST identifies function calls but does not define their
implementation.

Example:

```text
sin(x)
```

is represented as a FunctionCallNode.

The Function Registry determines whether the configured mathematical
library provides a function named `sin` and executes the associated
implementation.

If the function is unavailable, execution fails with an unsupported
function error.

Users are responsible for knowing the syntax and available functions
of the selected mathematical library.

The default mathematical library is based on `cmath`.

---

# Mathematical Libraries

Mathematical libraries provide concrete implementations of mathematical
functions.

Examples:

* standard C++ `cmath`;
* Boost mathematical functions;
* arbitrary precision libraries;
* user-defined libraries.

Each library may expose its own function vocabulary.

The Function Registry adapts the selected library to the numathap
execution model.

No prior function-name translation is required.

The expression syntax follows the selected mathematical library.

---

# Execution Backends

Backends operate on the Prepared-AST through the Dispatcher.

They define execution strategies while sharing the same mathematical
representation.

## Evaluator Backend

The Evaluator Backend performs numerical evaluation.

It uses:

* Dispatcher;
* Context;
* Value;
* Function Registry.

It is responsible for evaluating mathematical expressions but does not
provide mathematical functions itself.

---

## Symbolic Backends

Symbolic Backends operate on the Prepared-AST to produce symbolic
results.

Possible services include:

* symbolic differentiation;
* algebraic simplification;
* symbolic transformations.

---

# Mathematical Services

## Integrator

Performs numerical integration by repeatedly evaluating expressions
through the Evaluator Backend.

---

## Limit Calculator

Computes limits through successive evaluations and numerical techniques
using the shared evaluation infrastructure.

---

## Differentiator

Transforms a Prepared-AST into another representation containing the
symbolic derivative.

Example:

```text
x^2 + sin(x)
```

becomes:

```text
2*x + cos(x)
```

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
                       |
Input String             |
     |                   |
     v                   |
   Lexer                 |
     |                   |
     v                   |
  Parser                 |
     |                   |
     v                   |
 Parser-AST              |
     |                   |
     v                   |
 MathAstBuilder          |
     |                   |
     v                   |
  Math-AST --------------+
     |
     v
 Orchestrator
     |
     v
 Prepared-AST
     |
     v
 Dispatcher
     |
     +----------------+
     |                |
     v                v

 Symbolic          Evaluator
 Backends          Backend
                       |
              +--------+--------+
              |                 |
              v                 v
       FunctionRegistry       Context
              |
              v
       Selected Math Library
```

---

# Architecture Summary

## User Interface

* Session

## Configuration

* Configurator
* Math Environment
* Capabilities

## Representation

* Token
* Lexer
* Parser
* Parser-AST
* Math-AST

## Preparation

* Math-AST Builder
* Orchestrator
* Prepared-AST

## Shared Infrastructure

* Dispatcher
* Context
* Value
* Function Registry

## Execution

* Evaluator Backend
* Symbolic Backends
* Integrator
* Limit Calculator
* Differentiator
* Future mathematical services

This organization separates:

* syntax;
* mathematical representation;
* preparation;
* execution strategy;
* mathematical library integration.

The result is an extensible mathematical processing architecture where
new capabilities and mathematical libraries can be added without
changing the core expression model.
