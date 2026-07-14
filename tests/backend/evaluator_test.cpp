#include <iomanip>
#include <iostream>
#include <string>

#include "numathap/numathap.hpp"

int main() {
    using namespace numathap;

    Session session;

    auto expression = session.prepare("sin(x)+exp(y)");

    struct TestCase {
        const char* x;
        const char* y;
    };

    const TestCase tests[] = {{"0", "0"}, {"pi/2", "1"}, {"pi", "2"}};

    std::cout << std::setprecision(15);

    for (const auto& test : tests) {
        core::Context context;

        context.set("x", session.evaluate(test.x));

        context.set("y", session.evaluate(test.y));

        auto result = expression(context);

        std::cout << "sin(" << test.x << ") + exp(" << test.y
                  << ") = " << result.real().value() << '\n';
    }

    return 0;
}