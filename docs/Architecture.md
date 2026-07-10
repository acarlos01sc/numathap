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

# Orchestrator

The **Orchestrator** prepares a Math-AST for a particular backend.

Possible preparation steps include:

-   simplification;
-   normalization;
-   algebraic rewriting;
-   removal of redundant expressions;
-   structural optimizations.

The result is called the **Prepared-AST**.

Prepared-AST is a conceptual stage rather than necessarily a distinct
tree. It may be identical to the original Math-AST or a transformed
version suitable for the requested operation.

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

The **Function Registry** maps textual function names to executable
mathematical implementations.

This enables built-in and user-defined functions without modifying
either the parser or the Math-AST.

------------------------------------------------------------------------

# Backends

Backends assign mathematical meaning to a Prepared-AST using the shared
infrastructure.

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

Input String
     |
     v
Lexer
     |
     v
Tokens
     |
     v
Parser
     |
+----------------+
|  Parser-AST    |
+----------------+
     |
     v
MathAstBuilder
     |
+----------------+
|   Math-AST     |
+----------------+
     |
     v
+----------------+
| Orchestrator   |
+----------------+
     |
     +--> Simplifier Capability
     |
     +--> ConstantFolder Capability
     |
     +--> Normalizer Capability
     |
     v
+----------------+
| Prepared-AST   |
+----------------+
     |
     v
+-------------------+
|  NodeDispatcher   |
+-------------------+
     |
     +---------------------------+
     |                           |
     v                           v

Symbolic Backends             Evaluator
                              Backend
     |                           |
     |                           +--> Context
     |                           |
     |                           +--> FunctionRegistry
     |                           |
     |                           +--> Value
     |
     +--> AstPrinter
     +--> Differentiator
     +--> Analyzer


                 Evaluator
                    ^
                    |
        +-----------+-----------+
        |                       |
        v                       v
   Integrator            LimitCalculator
   Backend               Backend
------------------------------------------------------------------------

# Architecture Summary

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