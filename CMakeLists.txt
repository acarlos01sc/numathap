cmake_minimum_required(VERSION 3.20)

project(numathap
	VERSION 0.1.0
	LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#----------------------------------------------------------
# numathap library
#----------------------------------------------------------

# Definição modular
file(GLOB SOURCES_BACKEND CONFIGURE_DEPENDS "src/backend/*.cpp")
file(GLOB SOURCES_CONFIG CONFIGURE_DEPENDS "src/config/*.cpp")
file(GLOB SOURCES_CORE CONFIGURE_DEPENDS "src/core/*.cpp")
file(GLOB SOURCES_DISPATCH CONFIGURE_DEPENDS "src/dispatch/*.cpp")
file(GLOB SOURCES_MATH CONFIGURE_DEPENDS "src/math/*.cpp")
file(GLOB SOURCES_NUMERIC CONFIGURE_DEPENDS "src/numeric/*.cpp")
file(GLOB SOURCES_ORCH CONFIGURE_DEPENDS "src/orchestration/*.cpp")
file(GLOB SOURCES_PARSER CONFIGURE_DEPENDS "src/parser/*.cpp")
file(GLOB SOURCES_SYMBOLIC CONFIGURE_DEPENDS "src/symbolic/*.cpp")

add_library(numathap

	# backend
	${SOURCES_BACKEND}

	# config
	${SOURCES_CONFIG}

	# core
	${SOURCES_CORE}

	# dispatch
	${SOURCES_DISPATCH}

	# math
	${SOURCES_MATH}

	# numeric
	${SOURCES_NUMERIC}

	# orchestration
	${SOURCES_ORCH}

	# parser
	${SOURCES_PARSER}

	# parser
	${SOURCES_SYMBOLIC}
)

set_target_properties(numathap
	PROPERTIES POSITION_INDEPENDENT_CODE ON
)

# 3. Melhorar warnings e flags
if(MSVC)
	target_compile_options(numathap PRIVATE /W4)
else()
	target_compile_options(numathap PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_include_directories(numathap
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#----------------------------------------------------------
# ParserAst_test
#----------------------------------------------------------

add_executable(ParserAst_test
	tests/parser/ParserAst_test.cpp
)

target_link_libraries(ParserAst_test PRIVATE numathap)

#----------------------------------------------------------
# Frontend_test
#----------------------------------------------------------

add_executable(frontend_test
	tests/parser/frontend_test.cpp
)

target_link_libraries(frontend_test PRIVATE numathap)

#----------------------------------------------------------
# PrepAst_print_test
#----------------------------------------------------------

add_executable(PrepAst_print_test
	tests/math/PrepAst_print_test.cpp
)

target_link_libraries(PrepAst_print_test
	PRIVATE numathap
)

#----------------------------------------------------------
# evaluate_test
#----------------------------------------------------------

add_executable(evaluate_test
	tests/math/evaluate_test.cpp
)

target_link_libraries(evaluate_test
	PRIVATE numathap
)

#----------------------------------------------------------
# simplifier_test
#----------------------------------------------------------

add_executable(simplifier_test
	tests/orchestration/simplifier_test.cpp
)

target_link_libraries(simplifier_test
	PRIVATE numathap
)

#----------------------------------------------------------
# mathenvironment_test
#----------------------------------------------------------

add_executable(mathenvironment_test
	tests/orchestration/mathenvironment_test.cpp
)

target_link_libraries(mathenvironment_test
	PRIVATE numathap
)

#----------------------------------------------------------
# mathenvironment_test
#----------------------------------------------------------

add_executable(configure_test
	tests/orchestration/configure_test.cpp
)

target_link_libraries(configure_test
	PRIVATE numathap
)



option(NUMATHAP_BUILD_PYTHON "Build Python bindings" OFF)

if(NUMATHAP_BUILD_PYTHON)
	add_subdirectory(python)
endif()