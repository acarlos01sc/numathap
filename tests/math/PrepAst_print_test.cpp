#include <iostream>

#include "numathap/numathap.hpp"

int main() {
    using namespace numathap;
    auto ast = numathap::prepare("sin(x)^2 + cos(x)^2");

    ast.print(std::cout);
    std::cout << '\n';

    return 0;
}