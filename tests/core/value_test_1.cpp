#include <iostream>

#include "numathap/core/Value.hpp"

int main() {
    using numathap::core::Value;

    std::cout << "Testing Value...\n\n";

    Value a(10.0);
    Value b(3.0);

    std::cout << "a = " << a.real().value() << '\n';
    std::cout << "b = " << b.real().value() << '\n';

    auto sum = a + b;
    auto sub = a - b;
    auto mul = a * b;
    auto div = a / b;

    std::cout << "\nArithmetic operations:\n";

    std::cout << "a + b = "
              << sum.real().value()
              << '\n';

    std::cout << "a - b = "
              << sub.real().value()
              << '\n';

    std::cout << "a * b = "
              << mul.real().value()
              << '\n';

    std::cout << "a / b = "
              << div.real().value()
              << '\n';


    std::cout << "\nComparison:\n";

    Value c(10.0);

    std::cout << "a == c : "
              << (a == c)
              << '\n';

    std::cout << "a != b : "
              << (a != b)
              << '\n';


    std::cout << "\nValue test finished.\n";

    return 0;
}