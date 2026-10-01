from importlib.metadata import version

from ._numathap import (
    Algorithm,
    Capability,
    Context,
    MathEnvironment,
    MathLibrary,
    NumericType,
    Value,
    AdaptiveSimpsonConfig,
    GaussKronrod15Config,
    configure,
    evaluate,
    evaluate_cx,
    integrate,
    differentiate,
    series,
    prepare,
)

__version__ = version("numathap")

__all__ = [
    "Algorithm",
    "Capability",
    "Context",
    "MathEnvironment",
    "MathLibrary",
    "NumericType",
    "Value",
    "AdaptiveSimpsonConfig",
    "GaussKronrod15Config",
    "configure",
    "evaluate",
    "evaluate_cx",
    "integrate",
    "differentiate",
    "series",
    "prepare",
]