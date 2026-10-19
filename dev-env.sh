#!/usr/bin/env bash

PROJECT_ROOT="$(pwd)"

source "$PROJECT_ROOT/.venv/bin/activate"

export PYTHONPATH="$PROJECT_ROOT/python/numathap${PYTHONPATH:+:$PYTHONPATH}"