cmake_minimum_required(VERSION 3.20)

project(
    numathap
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(numathap
    src/parser/Lexer.cpp
    src/parser/Parser.cpp
    src/numeric/Real.cpp
    src/core/Value.cpp
    src/core/Context.cpp
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

add_executable(lexer_test
    tests/parser/lexer_test.cpp
)

target_link_libraries(lexer_test PRIVATE numathap)

add_test(NAME lexer_test COMMAND lexer_test)