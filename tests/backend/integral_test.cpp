#include <iostream>
#include "numathap/numathap.hpp"

int main() {

    using namespace numathap;

    Context ctx;
    ctx.setInterval("x", "0", "pi");
    auto expr=prepare("sin(x)");
    auto result = integrate(expr,"x",ctx);

    std::cout << "Resultado : " << result << std::endl;

    return 0;

}