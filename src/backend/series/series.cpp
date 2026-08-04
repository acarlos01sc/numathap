/**
 * @file series.cpp
 * @brief Implements the public series entry points.
 */

#include "numathap/backend/series/series.hpp"

#include <stdexcept>

#include "numathap/backend/series/Taylor.hpp"
#include "numathap/config/MathEnvironment.hpp"

namespace numathap::backend::series {

math::PreparedAst series(const math::PreparedAst& prepared,
                         const std::string& variable,
                         const std::string& center) {
    config::MathEnvironment environment;

    core::Context context;

    return series(prepared, variable, center, context, environment);
}

math::PreparedAst series(const math::PreparedAst& prepared,
                         const std::string& variable,
                         const std::string& center,
                         const core::Context& context) {
    config::MathEnvironment environment;

    return series(prepared, variable, center, context, environment);
}

math::PreparedAst series(const math::PreparedAst& prepared,
                         const std::string& variable,
                         const std::string& center,
                         const config::MathEnvironment& environment) {
    core::Context context;

    return series(prepared, variable, center, context, environment);
}

math::PreparedAst series(const math::PreparedAst& prepared,
                         const std::string& variable,
                         const std::string& center,
                         const core::Context& context,
                         const config::MathEnvironment& environment) {
    (void)prepared;
    (void)variable;
    (void)center;
    (void)context;
    (void)environment;

    throw std::logic_error(
        "Series backend is not implemented yet.");
}

}  // namespace numathap::backend::series