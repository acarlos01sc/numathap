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

        constexpr std::size_t order = 9;

        const auto center = numathap::core::Value::parse("0");

        const auto derivatives =
            dag.evaluateDerivativesAt("x", center, order);

        std::cout << "Derivatives at x = 0:\n";

        for (std::size_t k = 0; k < derivatives.size(); ++k) {
            std::cout << "f[" << k << "](0) = "
                      << derivatives[k] << '\n';
        }

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}