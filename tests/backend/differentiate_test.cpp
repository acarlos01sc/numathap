#include <iostream>

#include "numathap/numathap.hpp"

int main() {

    using namespace numathap;

    Context ctx;
    auto expr = prepare("sin(x)");
    ctx.setValue("x","0");
    auto diff = differentiate(expr,"x");
    auto resp = evaluate(diff,ctx);

    std::cout << "Resultado : " << resp << std::endl;

    return 0;

}