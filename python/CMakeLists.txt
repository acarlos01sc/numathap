cmake_minimum_required(VERSION 3.20)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(_numathap
    bindings/module.cpp
    bindings/MathEnvironment.cpp
    bindings/Value.cpp
    bindings/Context.cpp
    bindings/prepare.cpp
    bindings/evaluate.cpp
)

target_link_libraries(_numathap
    PRIVATE
        numathap
)