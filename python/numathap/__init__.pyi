# python/numathap/__init__.pyi

from __future__ import annotations
from typing import Any, Optional, overload
from enum import Enum


class _PreparedAst:
    """
    Internal parsed representation of a mathematical expression.

    Usually created by prepare() and passed to evaluate(),
    integrate() or differentiate().
    """

    def expression(self) -> str:
        """
        Return the original expression string.
        """
        ...

    def empty(self) -> bool:
        """
        Check whether this AST is empty.
        """
        ...

    def print(self) -> str:
        """
        Return a textual AST representation.
        """
        ...


class MathEnvironment:
    """
    Controls expression evaluation configuration.
    """

    def __init__(self) -> None:
        ...

    @property
    def math_library(self) -> MathLibrary:
        ...

    @property
    def numeric_type(self) -> NumericType:
        ...


@overload
def prepare(expression: str) -> _PreparedAst:
    """
    Prepare a mathematical expression using the default environment.

    Args:
        expression: Mathematical expression string.

    Returns:
        Prepared expression AST.
    """
    ...


@overload
def prepare(
    expression: str,
    environment: MathEnvironment
) -> _PreparedAst:
    """
    Prepare a mathematical expression using a custom environment.

    Args:
        expression:
            Mathematical expression string.

        environment:
            Environment controlling available functions,
            constants and capabilities.

    Returns:
        Prepared expression AST.
    """
    ...


class Context:
    """
    Runtime context for expressions.
    """

    def __init__(self) -> None:
        ...

    def set_value(
        self,
        symbol: str,
        definition: str
    ) -> None:
        ...

    def find_value(
        self,
        symbol: str
    ) -> Optional[str]:
        ...

    def has_value(
        self,
        symbol: str
    ) -> bool:
        ...

    def set_interval(
        self,
        symbol: str,
        lower: str,
        upper: str
    ) -> None:
        ...

    def find_interval(
        self,
        symbol: str
    ) -> Any:
        ...

    def has_interval(
        self,
        symbol: str
    ) -> bool:
        ...

    def clear(self) -> None:
        ...


class Value:
    """
    Numeric value returned by numathap computations.
    """

    @overload
    def __init__(self) -> None:
        ...

    @overload
    def __init__(self, value: float) -> None:
        ...

    def real(self) -> float:
        ...

def evaluate(
    expression: _PreparedAst,
    context: Context
) -> Value:
    """
    Evaluate a prepared expression.
    """
    ...

@overload
def integrate(
    expression: _PreparedAst,
    variable: str,
    context: Context
) -> Value:
    ...


@overload
def integrate(
    expression: _PreparedAst,
    variable: str,
    context: Context,
    environment: MathEnvironment
) -> Value:
    ...

def differentiate(
    expression: _PreparedAst,
    variable: str
) -> _PreparedAst:
    ...


class Algorithm(Enum):
    AdaptiveSimpson = ...
    GaussKronrod15 = ...



class Capability(Enum):
    Simplify = ...


class MathLibrary(Enum):
    CMath = ...


class NumericType(Enum):
    Double = ...


class AdaptiveSimpsonConfig:
    tolerance: Value
    maxDepth: int


class GaussKronrod15Config:
    absoluteTolerance: Value
    relativeTolerance: Value
    maxEvaluations: int