# Numathap Architecture Overview

## Overview

**numathap** is a library designed to provide a high-level API for mathematical and numerical computations in **C++** and **Python**.

Its main purpose is to allow mathematical expressions to be represented as strings and used as input for different processing services, such as numerical evaluation, numerical integration, limit calculation, symbolic differentiation, simplification, and other capabilities that may be incorporated in the future.

The architecture of numathap is inspired by the traditional structure of compilers, clearly separating the stages of **expression analysis**, **internal representation**, and **execution of mathematical operations**.

This separation allows different algorithms to use the same mathematical representation without making the expression structure dependent on a specific purpose.

---

# Expression Representation and Analysis

The initial processing of an expression occurs through a sequence of well-defined stages.

## Token

A **Token** represents a lexical unit of the mathematical language, such as:

* numbers;
* identifiers;
* operators;
* delimiters;
* special symbols recognized by the library.

Each token contains the information required by the subsequent processing stages.

---

## Lexer

The **Lexer** performs lexical analysis of the input expression.

Its responsibility is to convert a sequence of characters into an organized sequence of tokens, identifying the individual elements that compose a mathematical expression.

Example:

Input:

```text
sin(x) + 2*x
```

Conceptual output:

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

## Node

A **Node** represents the elements that compose the **Abstract Syntax Tree (AST)**.

Each mathematical construction has a corresponding node type, such as:

* numeric values;
* identifiers;
* unary operators;
* binary operators;
* function calls;
* other mathematical structures.

AST nodes are exclusively responsible for representing the expression structure.

They do not perform calculations and do not contain interpretation-specific logic.

---

## Parser

The **Parser** performs syntactic analysis of the token sequence and builds the Abstract Syntax Tree (AST).

During this stage, the grammar rules of the mathematical language are verified, ensuring that the expression has a valid structure.

Example:

Expression:

```text
sin(x) + x^2
```

is converted into a hierarchical structure similar to:

```text
          +
         / \
      sin   ^
       |   / \
       x  x   2
```

---

# Abstract Syntax Tree (AST)

After parsing is complete, no mathematical operation has been executed.

The AST exclusively represents the structure of the expression, independently of the algorithm that will be applied later.

This characteristic is fundamental to the numathap architecture:

* the AST does not know how an expression should be evaluated;
* the AST does not know how an integral should be calculated;
* the AST does not know how a derivative should be obtained;
* the AST does not know how simplification should be performed.

Expression interpretation is the responsibility of specialized components.

The same AST can be used by different services:

* numerical evaluation;
* integration;
* limit calculation;
* symbolic differentiation;
* algebraic simplification;
* optimization.

---

# Processing Components

## Orchestrator

The **Orchestrator** is responsible for coordinating AST preparation before a specific operation is executed.

It receives an AST produced by the Parser and may apply intermediate transformation stages, such as:

* simplification;
* normalization;
* elimination of redundant expressions;
* structural optimizations.

Initially, the Orchestrator may simply provide the original AST, acting as an extension point for future preparation stages.

Its purpose is to provide backends with an AST suitable for the requested processing task.

---

## Dispatcher

The **Dispatcher** is the mechanism responsible for routing operations to the appropriate components according to the node types found in the AST.

It allows different algorithms to process the same data structure without adding behavior to the tree nodes.

Therefore:

* the AST remains a purely representational structure;
* algorithms remain external to the nodes;
* new behaviors can be added without modifying the existing representation.

The Dispatcher is a fundamental layer enabling multiple backends to operate on the same AST.

---

## Context

The **Context** represents the environment required during expression processing.

It provides information external to the AST, such as:

* values associated with identifiers;
* user-defined variables;
* constants;
* other data required during execution.

For example, an expression:

```text
x^2 + sin(x)
```

does not contain enough information to be numerically evaluated.

The Context provides the value of `x` required by the backend to perform the computation.

---

# Backends

**Backends** are specialized services responsible for assigning mathematical meaning to the AST.

They use the common infrastructure provided by the library, especially:

* Dispatcher;
* Context;
* Value.

Each backend implements a specific capability.

---

## ExpressionEvaluator

The **ExpressionEvaluator** is the backend responsible for direct numerical interpretation of the AST.

It performs:

* operator resolution;
* numerical value evaluation;
* variable resolution through Context;
* execution of mathematical function calls.

The result of this process is represented by an object of type `Value`.

Example:

Input:

```text
sin(x) + x^2
```

Context:

```text
x = 2
```

Result:

```text
Value(4.909...)
```

---

## Integrator

The **Integrator** is the backend responsible for numerical integration.

During its processing, it may use the ExpressionEvaluator to evaluate the expression at different points in the integration domain.

Example:

```text
∫ sin(x) dx
```

The integrator controls the numerical method, while expression evaluation remains the responsibility of the ExpressionEvaluator.

---

## LimitCalculator

The **LimitCalculator** is the backend responsible for computing limits.

It uses successive evaluations of an expression to determine the behavior of a function in a specific region.

Like the Integrator, it may use the ExpressionEvaluator as its numerical evaluation mechanism.

---

## Differentiator

The **Differentiator** is a symbolic backend responsible for transforming an AST into a new AST representing the derivative of the original expression.

Unlike numerical backends, its output is not necessarily a `Value`, but another mathematical representation.

Example:

Input:

```text
x^2 + sin(x)
```

Result:

```text
2*x + cos(x)
```

---

# Value

The **Value** type represents the abstraction of values manipulated by the numerical components of the library.

It decouples mathematical algorithms from the internal representation of numbers.

This abstraction allows future evolution, such as:

* higher numerical precision;
* complex numbers;
* alternative representations;
* specialized numerical types.

---

# Function Registry

Mathematical functions used in expressions are handled through a dedicated function registration and resolution layer.

This layer maps the textual representation of a function to its corresponding mathematical implementation.

Examples:

```text
sin(x)
cos(x)
log(x)
sqrt(x)
```

The ExpressionEvaluator requests function execution, while the resolution of the concrete implementation remains decoupled.

This approach allows future extensions such as:

* adding new mathematical functions;
* registering user-defined functions;
* creating extensions without modifying the parser or AST.

---

# General Architecture Flow

The complete expression processing flow is:

```text
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
      v
     AST
      |
      v
 Orchestrator
      |
      v
 Prepared AST
      |
      v
   Backend
      |
      v
 Dispatcher
      |
      v
Context + Value
```

---

# Final Considerations

The numathap architecture clearly separates three fundamental responsibilities:

## Representation

Responsible for describing the mathematical expression:

```text
Token
Lexer
Parser
AST
```

## Preparation

Responsible for transforming the representation before processing:

```text
Orchestrator
Simplifiers
Optimizers
```

## Execution

Responsible for assigning mathematical meaning:

```text
Dispatcher
Backends
Context
Value
```

This organization allows numathap to evolve from a simple expression evaluator into an extensible mathematical processing platform, while keeping expression representation, calculation algorithms, and numerical execution independent from each other.
