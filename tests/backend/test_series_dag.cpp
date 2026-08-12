#include <iostream>
#include <string>

#include "numathap/backend/series/SeriesDag.hpp"
#include "numathap/math/PreparedAst.hpp"
#include "numathap/numathap.hpp"

int main() {
    try {
        const std::string expression = "x/sqrt(1-x^2)";

        auto prepared = numathap::prepare(expression);

        std::cout << "Expression:\n" << expression << "\n\n";

        std::cout << "Prepared AST:\n";
        prepared.print(std::cout);
        std::cout << '\n';

        numathap::backend::series::SeriesDag dag(prepared);

        auto d1 = dag.differentiate("x");
        auto d2 = d1.differentiate("x");
        auto d3 = d2.differentiate("x");
        auto d4 = d3.differentiate("x");
        auto d5 = d4.differentiate("x");
        auto d6 = d5.differentiate("x");
        auto d7 = d6.differentiate("x");
        auto d8 = d7.differentiate("x");
        auto d9 = d8.differentiate("x");

        std::cout << "Derivada 1: " << d1.nodeCount() << std::endl;
        std::cout << "Derivada 2: " << d2.nodeCount() << std::endl;
        std::cout << "Derivada 3: " << d3.nodeCount() << std::endl;
        std::cout << "Derivada 4: " << d4.nodeCount() << std::endl;
        std::cout << "Derivada 5: " << d5.nodeCount() << std::endl;
        std::cout << "Derivada 6: " << d6.nodeCount() << std::endl;
        std::cout << "Derivada 7: " << d7.nodeCount() << std::endl;
        std::cout << "Derivada 8: " << d8.nodeCount() << std::endl;
        std::cout << "Derivada 9: " << d9.nodeCount() << std::endl;

        auto ctx = numathap::Context();
        ctx.setValue("x", "0");
        std::cout << "Termo 1: " << d1.evaluate(ctx) << std::endl;
        std::cout << "Termo 2: " << d2.evaluate(ctx) << std::endl;
        std::cout << "Termo 3: " << d3.evaluate(ctx) << std::endl;
        std::cout << "Termo 4: " << d4.evaluate(ctx) << std::endl;
        std::cout << "Termo 5: " << d5.evaluate(ctx) << std::endl;
        std::cout << "Termo 6: " << d6.evaluate(ctx) << std::endl;
        std::cout << "Termo 7: " << d7.evaluate(ctx) << std::endl;
        std::cout << "Termo 8: " << d8.evaluate(ctx) << std::endl;
        std::cout << "Termo 9: " << d9.evaluate(ctx) << std::endl;

        //        std::cout << "DAG:\n";
        //        dag.print(std::cout);
        //
        //        std::cout << "\nDAG node count: " << dag.nodeCount() << "\n";
        //
        //        auto derivative = dag.differentiate("x");
        //
        //        std::cout << "\nDerivative DAG:\n";
        //        derivative.print(std::cout);
        //
        //        std::cout << "\nDerivative DAG node count: " <<
        //        derivative.nodeCount()
        //                  << "\n";
        //
        //        auto derivative2 = derivative.differentiate("x");
        //
        //        std::cout << "\nDerivative 2 DAG:\n";
        //        derivative2.print(std::cout);
        //
        //        std::cout << "\nDerivative2 DAG node count: " <<
        //        derivative2.nodeCount()
        //                  << "\n";

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';

        return 1;
    }
}