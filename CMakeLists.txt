cmake_minimum_required(VERSION 3.20)

project(
    numathap
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(numathap

    # Public API
    src/Session.cpp
    src/Context.cpp

    # Configuration
    src/config/Configurator.cpp
    src/config/MathAdapter.cpp
    src/config/MathEnvironment.cpp
    src/config/CMathAdapter.cpp

    # Backend
    src/backend/Evaluator.cpp

    # Parser
    src/parser/Lexer.cpp
    src/parser/Parser.cpp
    src/parser/AstPrinter.cpp

    # Mathematical Representation
    src/math/MathAstBuilder.cpp
    src/math/MathAstPrinter.cpp

    # Numeric
    src/numeric/Real.cpp

    # Core
    src/core/Context.cpp
    src/core/Value.cpp

    # Orchestration
    src/orchestration/Orchestrator.cpp
)

target_compile_features(numathap PUBLIC cxx_std_20)

target_include_directories(numathap
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(MSVC)
    target_compile_options(numathap PRIVATE /W4)
else()
    target_compile_options(numathap PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

enable_testing()

#
# Tests
#



add_executable(lexer_test
    tests/parser/lexer_test.cpp
)

target_link_libraries(lexer_test PRIVATE numathap)

add_test(
    NAME lexer_test
    COMMAND lexer_test
    )


add_executable(pipeline_test
    tests/parser/pipeline_test.cpp
)

target_link_libraries(pipeline_test PRIVATE numathap)

add_test(NAME pipeline_test COMMAND pipeline_test)

add_executable(evaluator_test
    tests/backend/evaluator_test.cpp
)

target_link_libraries(evaluator_test PRIVATE numathap)