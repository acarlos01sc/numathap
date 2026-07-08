cmake_minimum_required(VERSION 3.20)

project(
    numathap
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_EXTENSIONS OFF)

add_library(numathap
    src/parser/Lexer.cpp
    src/parser/Parser.cpp
    src/ast/AstPrinter.cpp
    src/numeric/Real.cpp
    src/core/Value.cpp
    src/core/Context.cpp
    src/core/Evaluator.cpp
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

add_executable(lexer_test
    tests/parser/lexer_test.cpp
)

target_link_libraries(lexer_test PRIVATE numathap)

add_test(NAME lexer_test COMMAND lexer_test)

add_executable(parser_ast_test
    tests/parser/parser_ast_test.cpp
)

target_link_libraries(parser_ast_test PRIVATE numathap)

add_test(NAME parser_ast_test COMMAND parser_ast_test)

add_executable(value_test_1
    tests/core/value_test_1.cpp
)

target_link_libraries(value_test_1 PRIVATE numathap)

add_test(NAME value_test_1 COMMAND value_test_1)

add_executable(evaluator_test
    tests/core/evaluator_test.cpp
)

target_link_libraries(evaluator_test PRIVATE numathap)

add_test(NAME evaluator_test COMMAND evaluator_test)