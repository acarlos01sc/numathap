#include <iostream>

#include "numathap/numathap.hpp"

int main() {

    using namespace numathap;
    
    Context ctx;
    auto prep = prepare("sin(x)");
    ctx.setValue("x", "pi");
    auto resp = evaluate(prep, ctx);

    std::cout << "Resultado: " << resp << std::endl;
    return 0;
}