# numathap C++ SDK

The numathap C++ SDK provides a CMake-based interface for using the numathap numerical mathematics library from external C++ projects.

The SDK exposes the public numathap API through:

```cpp
#include <numathap/numathap.hpp>
```

## Requirements

* CMake 3.20 or later
* A C++20-compatible compiler
* Linux, macOS, or Windows

The current development environment is based on GCC 13 and Ubuntu Linux.

## Building numathap

Clone the repository and configure the project:

```bash
git clone <repository-url>
cd numathap

cmake -S . -B build
```

Build the library:

```bash
cmake --build build
```

Run the test suite:

```bash
ctest --test-dir build --output-on-failure
```

## Installing the C++ SDK

The SDK can be installed using CMake:

```bash
cmake --install build --prefix <installation-prefix>
```

For example:

```bash
cmake --install build --prefix /tmp/numathap-install
```

The installation contains the public headers, library, and CMake package configuration files.

The resulting structure is approximately:

```text
<installation-prefix>/
├── include/
│   └── numathap/
│       ├── numathap.hpp
│       └── ...
├── lib/
│   ├── libnumathap.a
│   └── cmake/
│       └── numathap/
│           ├── numathapConfig.cmake
│           └── numathapTargets.cmake
└── ...
```

## Using numathap from another CMake project

After installing the SDK, an external CMake project can locate it using `find_package()`.

Example `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.20)

project(numathap_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(numathap REQUIRED)

add_executable(example main.cpp)

target_link_libraries(example
    PRIVATE
        numathap::numathap
)
```

If the SDK was installed into a non-standard location, specify the installation prefix when configuring the project:

```bash
cmake -S . -B build \
    -DCMAKE_PREFIX_PATH=/tmp/numathap-install
```

Then build the application:

```bash
cmake --build build
```

## Basic Usage

The public API is available through `numathap/numathap.hpp`.

For example:

```cpp
#include <iostream>

#include <numathap/numathap.hpp>

int main()
{
    auto expr = numathap::prepare("sin(x)");

    numathap::Context ctx;
    ctx.setValue("x", "pi/2");

    auto result = numathap::evaluate(expr, ctx);

    std::cout << "Result: " << result << '\n';

    return 0;
}
```

The program produces:

```text
Result: 1
```

The example demonstrates the basic numathap processing flow:

1. Prepare a mathematical expression.
2. Create an evaluation context.
3. Define values used by the expression.
4. Evaluate the prepared expression.

## Numerical Integration

The C++ API also provides numerical integration:

```cpp
#include <iostream>

#include <numathap/numathap.hpp>

int main()
{
    auto expr = numathap::prepare("sin(x)");

    numathap::Context ctx;
    ctx.setInterval("x", "0", "pi");

    auto result = numathap::integrate(expr, "x", ctx);

    std::cout << "Integral: " << result << '\n';

    return 0;
}
```

The expected result is approximately:

```text
Integral: 2
```

## Public API

The main public namespace is:

```cpp
namespace numathap
```

The main types and functions currently exposed by `numathap/numathap.hpp` include:

### Expression preparation

```cpp
numathap::prepare(...)
```

### Numerical evaluation

```cpp
numathap::evaluate(...)
```

### Numerical integration

```cpp
numathap::integrate(...)
```

### Configuration

```cpp
numathap::configure(...)
```

### Core types

```cpp
numathap::PreparedAst
numathap::Context
numathap::Value
numathap::MathEnvironment
numathap::MathLibrary
numathap::NumericType
```

The `numathap.hpp` header is intended to be the primary entry point for external C++ applications.

## Python Bindings

numathap also provides optional Python bindings.

The Python bindings are not required to build or use the C++ SDK.

They can be enabled during configuration with:

```bash
cmake -S . -B build \
    -DNUMATHAP_BUILD_PYTHON=ON
```

The Python interface is maintained separately from the C++ SDK installation interface.

## Project Status

numathap is currently under active development.

The public C++ API and numerical backends are still evolving. Until the first stable release, API changes may occur between versions.

The current project version is:

```text
0.1.0
```

## License

See the repository's `LICENSE` file for licensing information.
