cmake_minimum_required(VERSION 3.20)

project(numathap_tests
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#----------------------------------------------------------
# numathap library
#----------------------------------------------------------

add_library(numathap

    #backend
    src/backend/Evaluator.cpp

    #math
    src/math/prepare.cpp
    src/math/PreparedAst.cpp
    src/math/MathAstBuilder.cpp
    src/math/MathAst.cpp
    src/math/MathAstPrinter.cpp

    #core
    src/core/Value.cpp
    src/core/Context.cpp

    #numeric
    src/numeric/Real.cpp 

    #dispatch
    src/dispatch/Dispatcher.cpp

    #parser
    src/parser/Parser.cpp
    src/parser/Lexer.cpp
    src/parser/ParserAst.cpp
    src/parser/ParserAstPrinter.cpp

    #orchestration
    src/orchestration/Orchestrator.cpp

    #config
    src/config/Configurator.cpp 
    src/config/MathEnvironment.cpp
    src/config/MathAdapter.cpp
    src/config/CMathDoubleAdapter.cpp

)

target_include_directories(numathap
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


#----------------------------------------------------------
# ParserAst_test
#----------------------------------------------------------

add_executable(ParserAst_test
    tests/parser/ParserAst_test.cpp

    src/parser/ParserAst.cpp
    src/parser/ParserAstPrinter.cpp
    src/parser/Parser.cpp
    src/parser/Lexer.cpp
)

target_include_directories(ParserAst_test
    PRIVATE
        include
)

#----------------------------------------------------------
# Frontend_test
#----------------------------------------------------------

add_executable(frontend_test
    tests/parser/frontend_test.cpp

    # Parser
    src/parser/ParserAst.cpp
    src/parser/ParserAstPrinter.cpp
    src/parser/Parser.cpp
    src/parser/Lexer.cpp

    # Math
    src/math/MathAst.cpp
    src/math/MathAstBuilder.cpp
    src/math/MathAstPrinter.cpp
)

target_include_directories(frontend_test
    PRIVATE
        include
)

#----------------------------------------------------------
# PrepAst_print_test
#----------------------------------------------------------

add_executable(PrepAst_print_test
    tests/math/PrepAst_print_test.cpp
)

target_link_libraries(PrepAst_print_test
    PRIVATE
        numathap
)