/**
 * @file BackendSupport.cpp
 * @brief Implements shared backend utility services.
 */

#include "numathap/backend/BackendSupport.hpp"

#include <stdexcept>

#include "numathap/backend/evaluate.hpp"
#include "numathap/math/prepare.hpp"
#include "numathap/numeric/Real.hpp"

namespace numathap::backend {

core::Value BackendSupport::evaluateConstant(
    const std::string& expression,
    const core::Context& context,
    const config::MathEnvironment& environment)
{
    //
    // 1. Numeric literal
    //
    try {
        std::size_t pos = 0;

        const auto value = std::stod(expression, &pos);

        if (pos == expression.size()) {
            return core::Value(numeric::Real::Storage(value));
        }

    } catch (...) {
    }

    //
    // 2. Mathematical constant
    //
    try {
        return environment.mathAdapter().resolveConstant(expression);

    } catch (...) {
    }

    //
    // 3. Constant expression
    //
    try {
        auto prepared = math::prepare(expression, environment);

        return backend::evaluate(prepared, context);

    } catch (...) {
        throw std::invalid_argument(
            "Invalid numeric value or constant expression: \"" +
            expression + "\"");
    }
}

}  // namespace numathap::backend