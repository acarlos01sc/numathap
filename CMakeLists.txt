cmake_minimum_required(VERSION 3.20)

project(
    numathap
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#
# Biblioteca
#
add_library(numathap
    src/parser/Lexer.cpp
    src/parser/Parser.cpp
)

target_include_directories(numathap
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

#
# Primeiro teste
#
add_executable(lexer_test
    tests/parser/lexer_test.cpp
)

target_link_libraries(lexer_test
    PRIVATE
        numathap
)