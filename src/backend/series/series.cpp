/**
 * @file series.cpp
 * @brief Implements the public series entry points.
 */

#include "numathap/backend/series/series.hpp"

#include <type_traits>
#include <variant>

#include "numathap/backend/BackendSupport.hpp"
#include "numathap/backend/series/Taylor.hpp"
#include "numathap/config/MathEnvironment.hpp"
#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"

namespace numathap::backend::series {

math::PreparedAst series(const math::PreparedAst& prepared,
                         const std::string& variable,
                         const std::string& center) {
    config::MathEnvironment environment;
    core::Context context;

    return series(prepared, variable, center, context, environment);
}

math::PreparedAst series(const math::PreparedAst& prepared,
                         const std::string& variable, const std::string& center,
                         const core::Context& context) {
    config::MathEnvironment environment;

    return series(prepared, variable, center, context, environment);
}

math::PreparedAst series(const math::PreparedAst& prepared,
                         const std::string& variable, const std::string& center,
                         const config::MathEnvironment& environment) {
    core::Context context;

    return series(prepared, variable, center, context, environment);
}

math::PreparedAst series(const math::PreparedAst& prepared,
                         const std::string& variable, const std::string& center,
                         const core::Context& context,
                         const config::MathEnvironment& environment) {

    return std::visit(
        [&](const auto& config) -> math::PreparedAst {
            using Config = std::decay_t<decltype(config)>;

            if constexpr (std::is_same_v<Config, TaylorConfig>) {
                return Taylor::series(prepared, variable,
                                      BackendSupport::evaluateConstant(
                                          center, context, environment),
                                      config);
            } else {
                static_assert(std::is_same_v<Config, void>,
                              "Unsupported series configuration.");
            }
        },
        environment.seriesConfig());
}

}  // namespace numathap::backend::series