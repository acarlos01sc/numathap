include(FetchContent)

# Download de uma versão específica do GoogleTest.
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(googletest)

# Diretório com os testes C++.
add_subdirectory(cpp)