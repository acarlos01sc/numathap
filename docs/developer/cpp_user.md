# numathap

**numathap** is a C++20 numerical mathematics library for parsing mathematical expressions and performing numerical computations.

This release provides the precompiled static library together with its public headers and CMake package configuration.

## Requirements

* C++20-compatible compiler
* CMake 3.20 or newer
* A supported C++ standard library

The current release provides a static library built for the target platform.

## Package contents

The release has the following structure:

```text
numathap/
├── include/
│   └── numathap/
│       ├── numathap.hpp
│       └── ...
└── lib/
    ├── libnumathap.a
    └── cmake/
        └── numathap/
            ├── numathapConfig.cmake
            ├── numathapTargets.cmake
            └── numathapTargets-release.cmake
```

The `include` directory contains the public C++ headers.

The `lib` directory contains the precompiled static library and the CMake package configuration.

## Using numathap with CMake

The recommended way to use numathap is through CMake.

Suppose the numathap release was extracted to:

```text
/path/to/numathap/
```

In your own project's `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.20)

project(example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(numathap REQUIRED)

add_executable(example main.cpp)

target_link_libraries(example PRIVATE numathap::numathap)
```

Configure your project by providing the numathap release directory through `CMAKE_PREFIX_PATH`:

```bash
cmake -S . -B build \
    -DCMAKE_PREFIX_PATH=/path/to/numathap
```

Then build:

```bash
cmake --build build
```

CMake will automatically locate the package configuration, public headers, and static library.

## Minimal example

Create a `main.cpp`:

```cpp
#include <numathap/numathap.hpp>

int main()
{
    auto ast = numathap::prepare("x^2");

    return 0;
}
```

The public API is available through:

```cpp
#include <numathap/numathap.hpp>
```

Applications should normally include this header rather than depending on the internal directory structure of the library.

## Using the library without CMake

It is also possible to use the release directly with a compiler.

For example:

```bash
g++ -std=c++20 main.cpp \
    -I/path/to/numathap/include \
    -L/path/to/numathap/lib \
    -lnumathap \
    -o example
```

Run the resulting program:

```bash
./example
```

For larger projects, CMake integration is recommended.

## CMake package location

The release contains the CMake package configuration at:

```text
lib/cmake/numathap/numathapConfig.cmake
```

Therefore, `CMAKE_PREFIX_PATH` must point to the **root of the numathap release**, not directly to its `lib` directory.

Correct:

```bash
-DCMAKE_PREFIX_PATH=/path/to/numathap
```

Not:

```bash
-DCMAKE_PREFIX_PATH=/path/to/numathap/lib
```

## Public API

The main public header is:

```cpp
#include <numathap/numathap.hpp>
```

The library currently provides functionality for:

* Mathematical expression parsing
* Mathematical AST construction
* Expression preparation
* Numerical evaluation
* Numerical integration
* Configurable mathematical environments
* Numerical and symbolic backends

See the project documentation for details about the available APIs and algorithms.

## Library type

This release provides:

```text
libnumathap.a
```

which is a static library.

The application is therefore linked against the precompiled numathap library at build time.

## Version

This release corresponds to:

```text
numathap 0.1.0
```

## License

See the project repository for licensing information.
