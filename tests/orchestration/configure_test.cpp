#include <iostream>

#include "numathap/numathap.hpp"

using namespace numathap;

int main()
{
    Context ctx;
    ctx.setValue("x", "pi/2");

    auto expr1 = prepare("sin(x)");
    auto result1 = evaluate(expr1, ctx);

    auto env = configure(
        config::Capability::Simplify);

    auto expr2 = prepare("sin(x)", env);
    auto result2 = evaluate(expr2, ctx);

    std::cout << "Without configure : "
              << result1 << '\n';

    std::cout << "With Simplify     : "
              << result2 << '\n';

    return 0;
}