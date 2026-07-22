
#include <iostream>

#include "numathap/config/Capability.hpp"
#include "numathap/numathap.hpp"

int main() {

    using namespace numathap;

    Context ctx;
    auto env = configure(config::Capability::Simplify);
    auto expr = prepare("1*x^2",env);
    ctx.setValue("x","pi");

    auto resp = evaluate(expr,ctx);

    std::cout << "Resultado : " << resp << std::endl;

    return 0;

}