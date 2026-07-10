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
introducing a clear distinction between **syntactic representation** and
**mathematical representation**. This separation allows different
algorithms to share the same mathematical model without coupling
expression representation to any specific computation.

------------------------------------------------------------------------
# Public API

The main user entry point is the **Session**.

A Session represents an isolated execution environment containing:

- mathematical library configuration;
- available capabilities;
- registered functions;
- execution services.

The user normally does not create internal components such as
Orchestrator, Dispatcher or FunctionRegistry directly.

Example:

```cpp
numathap::Session session;

auto result =
    session.evaluate("sin(x)+sqrt(x)");

Advanced users may customize the Session environment:

session.setMathLibrary("custom_library");

session.enableCapability(
    Capability::Simplifier
);

The default configuration uses the standard mathematical library
based on cmath.

-------------------------------------------------------------------------
# Expression Analysis

## Token

A **Token** represents a lexical element of the mathematical language,
including numbers, identifiers, operators, delimiters and special
symbols.

------------------------------------------------------------------------

## Lexer

The **Lexer** converts the input character stream into an ordered
sequence of tokens.

Example:

``` text
sin(x) + 2*x
```

becomes

``` text
FUNCTION(sin)
LPAREN
IDENTIFIER(x)
RPAREN
PLUS
NUMBER(2)
MULTIPLY
IDENTIFIER(x)
```

------------------------------------------------------------------------

## Parser-AST Nodes

Parser nodes represent the syntactic structure produced by the parser.

Each node models only grammar constructs such as:

-   numeric literals;
-   identifiers;
-   unary operators;
-   binary operators;
-   function calls;
-   other syntactic elements.

Parser nodes contain no evaluation logic.

------------------------------------------------------------------------

## Parser

The **Parser** validates the grammar and builds the **Parser-AST**.

Example:

``` text
sin(x) + x^2
```

``` text
        +
       / \
    sin   ^
     |   / \
     x  x   2
```

------------------------------------------------------------------------

# Parser-AST

The Parser-AST represents only the grammatical organization of the
expression.

It knows nothing about numerical evaluation, symbolic manipulation or
optimization.

Its only purpose is to preserve the parsed structure.

------------------------------------------------------------------------

# Math-AST Builder

The **Math-AST-Builder** converts the Parser-AST into a **Math-AST**.

While the Parser-AST models syntax, the Math-AST models mathematical
meaning. Equivalent syntactic constructions may therefore produce the
same mathematical representation.

The Math-AST is independent of the parser implementation and may
eventually be produced by other frontends.

------------------------------------------------------------------------

# Configuration System

The configuration system defines how a Session is assembled.

It separates user configuration from internal execution components.

## Configurator

The **Configurator** is responsible for creating and modifying the
mathematical execution environment.

It controls:

- selected mathematical library;
- available capabilities;
- precision options;
- backend configuration.

The Configurator does not execute mathematical operations.

Its role is to produce a configured environment.

## Math Environment

The **Math Environment** represents the resolved configuration used
during execution.

It contains:

- Math Backend;
- Function Registry;
- Capability set;
- execution options.

Runtime components receive the environment instead of directly
consulting the Configurator.
-------------------------------------------------------------------

# Orchestrator

The **Orchestrator** prepares a Math-AST using the capabilities enabled
by the current Math Environment.

It does not manage configuration.

The environment is created before execution by the configuration system
and provided to the Orchestrator.

Possible preparation steps include:

- simplification;
- normalization;
- algebraic rewriting;
- removal of redundant expressions;
- structural optimizations.

The result is the Prepared-AST.

------------------------------------------------------------------------

# Shared Infrastructure

## Dispatcher

The **Dispatcher** routes operations to the correct node-specific
implementation without embedding behavior into the node classes.

It allows multiple algorithms to operate on the same Math-AST while
keeping node classes purely structural.

## Context

The **Context** stores external information required during execution,
including variable values, constants and user-defined symbols.

## Value

The **Value** type abstracts the numeric objects manipulated by
numerical backends.

Future versions may support arbitrary precision, complex numbers and
additional numeric representations.

## Function Registry

The **Function Registry** maps textual function names to mathematical
implementations provided by the selected Math Library.

The Math-AST identifies function calls but does not define their
meaning.

Example:

```text
tb(x)

is represented as a FunctionCallNode.

The Function Registry determines whether the configured mathematical
library provides a function named tb.

If the function is not available, execution fails with an unsupported
function error.

Users are responsible for knowing the syntax and available functions
of the selected mathematical library.

The default mathematical library is based on cmath and its supported
functions are documented by the library.
------------------------------------------------------------------------

# Mathematical Libraries and Backends

Mathematical libraries provide concrete implementations of mathematical
functions.

Examples:

- standard C++ `cmath`;
- Boost mathematical functions;
- arbitrary precision libraries;
- user-defined libraries.

Each library may expose its own function vocabulary.

The Function Registry adapts the selected library to the numathap
execution model.

## ExpressionEvaluator

Evaluates expressions numerically using:

-   Dispatcher;
-   Context;
-   Value;
-   Function Registry.

## Integrator

Performs numerical integration by repeatedly evaluating the expression
throughout the integration domain using the library evaluation
infrastructure.

## LimitCalculator

Determines limits through successive evaluations and numerical
techniques while relying on the shared evaluation infrastructure.

## Differentiator

Transforms one Prepared-AST into another representing the symbolic
derivative.

Example:

``` text
x^2 + sin(x)
```

becomes

``` text
2*x + cos(x)
```

------------------------------------------------------------------------

# Processing Pipeline

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
 Parser                  |
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
                       |
              +--------+--------+
              |                 |
              v                 v
       FunctionRegistry       Context
              |
              v
       Math Backend
------------------------------------------------------------------------

# Architecture Summary

## User Interface

- Session

## Configuration

- Configurator
- Math Environment
- Math Backend
- Capabilities

## Representation

-   Token
-   Lexer
-   Parser
-   Parser-AST

## Preparation

-   Math-AST-Builder
-   Math-AST
-   Orchestrator
-   Prepared-AST

## Shared Infrastructure

-   Dispatcher
-   Context
-   Value
-   Function Registry

## Backends

-   ExpressionEvaluator
-   Integrator
-   LimitCalculator
-   Differentiator
-   Future mathematical services

This organization separates syntax, mathematical representation and
execution, allowing the library to evolve into an extensible
mathematical processing platform while keeping responsibilities clearly
isolated.