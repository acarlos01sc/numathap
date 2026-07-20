#include <iostream>

#include "numathap/numathap.hpp"

int main() {

    using namespace numathap;
    
    Context ctx;
    auto prep = prepare("sin(x)");
    ctx.setValue("x", "pi/2");
    auto resp = evaluate(prep, ctx);

    std::cout << "Resultado de sin(pi/2): " << resp << std::endl;

    return 0;
}