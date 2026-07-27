cmake_minimum_required(VERSION 3.20)

project(numathap
	VERSION 0.1.0
	LANGUAGES CXX
)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#----------------------------------------------------------
# numathap library
#----------------------------------------------------------

# Definição modular
file(GLOB SOURCES_BACKEND CONFIGURE_DEPENDS "src/backend/*.cpp")
file(GLOB SOURCES_INTEGRATE CONFIGURE_DEPENDS "src/backend/integrate/*.cpp")
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

	# symbolic
	${SOURCES_SYMBOLIC}

	# integrate
	${SOURCES_INTEGRATE}
)

set_target_properties(numathap
	PROPERTIES POSITION_INDEPENDENT_CODE ON
)

target_include_directories(numathap
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#----------------------------------------------------------
# viewer utility library
#----------------------------------------------------------

add_library(AstViewerCore STATIC
	tests/utils/viewer/src/PrinterTextParser.cpp
	tests/utils/viewer/src/AstViewerCore.cpp
)

target_include_directories(AstViewerCore
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tests/utils/viewer/include
)

target_link_libraries(AstViewerCore
	PUBLIC numathap
)

# 3. Melhorar warnings e flags
if(MSVC)
	target_compile_options(numathap PRIVATE /W4)
else()
	target_compile_options(numathap PRIVATE -Wall -Wextra -Wpedantic)
endif()

#----------------------------------------------------------
# viewer utility: PrinterTextParser_test
#----------------------------------------------------------
add_executable(PrinterTextParser_test
	tests/utils/viewer/src/PrinterTextParser_test.cpp
)

target_link_libraries(PrinterTextParser_test
	PRIVATE AstViewerCore
)

add_test(NAME PrinterTextParser_test COMMAND PrinterTextParser_test)

#----------------------------------------------------------
# viewer utility: PrinterTextParser_integration_test
#----------------------------------------------------------
add_executable(PrinterTextParser_integration_test
	tests/utils/viewer/src/PrinterTextParser_integration_test.cpp
)

target_link_libraries(PrinterTextParser_integration_test
	PRIVATE AstViewerCore
)

add_test(
	NAME PrinterTextParser_integration_test
	COMMAND PrinterTextParser_integration_test
)

#----------------------------------------------------------
# viewer utility: AstViewerCore_integration_test
#----------------------------------------------------------

add_executable(AstViewerCore_integration_test
	tests/utils/viewer/src/AstViewerCore_integration_test.cpp
)

target_link_libraries(AstViewerCore_integration_test
	PRIVATE AstViewerCore
)

add_test(
	NAME AstViewerCore_integration_test
	COMMAND AstViewerCore_integration_test
)

#----------------------------------------------------------
# ast_analyser: side-by-side AST inspection / --test snapshot tool
#----------------------------------------------------------

add_executable(ast_analyser
	tools/ast_analyser/ast_analyser.cpp
)

target_include_directories(ast_analyser
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tools/ast_analyser
)

target_link_libraries(ast_analyser
	PRIVATE numathap
)

if(MSVC)
	target_compile_options(ast_analyser PRIVATE /W4)
else()
	target_compile_options(ast_analyser PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_test(
	NAME ast_analyser_snapshot
	COMMAND ast_analyser
		${CMAKE_CURRENT_SOURCE_DIR}/tools/ast_analyser/examples/basic.yaml --test
)

#----------------------------------------------------------
# TEST_integrate_cfg
#----------------------------------------------------------

add_executable(TEST_integrate_cfg
	tests/config/TEST_integrate_cfg.cpp
)

target_include_directories(TEST_integrate_cfg
	PRIVATE
	 ${CMAKE_CURRENT_SOURCE_DIR}/tests/utils/viewer/include
	 ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(TEST_integrate_cfg
	PRIVATE numathap
)

add_test(
	NAME TEST_integrate_cfg
	COMMAND TEST_integrate_cfg
)


#----------------------------------------------------------
# TEST_integrate_eval
#----------------------------------------------------------

add_executable(TEST_integrate_eval
    tests/backend/TEST_integrate_eval.cpp
)

target_include_directories(TEST_integrate_eval
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/utils/viewer/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(TEST_integrate_eval
    PRIVATE numathap
)

add_test(
    NAME TEST_integrate_eval
    COMMAND TEST_integrate_eval
)

target_compile_definitions(TEST_integrate_eval
    PRIVATE
    NUMATHAP_TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/backend"
)


option(NUMATHAP_BUILD_PYTHON "Build Python bindings" OFF)

if(NUMATHAP_BUILD_PYTHON)
	add_subdirectory(python)
endif()