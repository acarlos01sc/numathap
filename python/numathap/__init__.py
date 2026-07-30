from importlib.metadata import version

from ._numathap import (
    Algorithm,
    Capability,
    Context,
    MathEnvironment,
    Value,
    configure,
    evaluate,
    integrate,
    prepare,
)

__version__ = version("numathap")

__all__ = [
    "Algorithm",
    "Capability",
    "Context",
    "MathEnvironment",
    "Value",
    "configure",
    "evaluate",
    "integrate",
    "prepare",
]