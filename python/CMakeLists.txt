cmake_minimum_required(VERSION 3.20)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(
	_numathap
	bindings/module.cpp
	bindings/MathEnvironment.cpp
	bindings/Value.cpp
	bindings/Context.cpp
	bindings/prepare.cpp
	bindings/evaluate.cpp
	bindings/Capability.cpp
	bindings/configure.cpp
)

set_target_properties(_numathap
	PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/numathap
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/numathap
)

target_link_libraries(_numathap
	PRIVATE numathap
)