cmake_minimum_required(VERSION 3.20)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development.Module)

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(
    _numathap
    
    bindings/module.cpp

    # Configuration
    bindings/MathEnvironment.cpp
    bindings/Capability.cpp
    bindings/configure.cpp

    # Core
    bindings/Value.cpp
    bindings/Context.cpp

    # Expression processing
    bindings/prepare.cpp
    bindings/evaluate.cpp

    # Numerical integration
    bindings/Algorithm.cpp
    bindings/integrate.cpp

    # Differentiate
    bindings/differentiate.cpp

    # Series
    bindings/series.cpp
)

set_target_properties(_numathap
	PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/numathap
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/numathap
)

target_link_libraries(_numathap
	PRIVATE numathap
)

include(GNUInstallDirs)

install(
    TARGETS _numathap
    LIBRARY DESTINATION numathap
    RUNTIME DESTINATION numathap
)

install(
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/numathap/__init__.py
        ${CMAKE_CURRENT_SOURCE_DIR}/numathap/__init__.pyi
        ${CMAKE_CURRENT_SOURCE_DIR}/numathap/py.typed
    DESTINATION
        numathap
)