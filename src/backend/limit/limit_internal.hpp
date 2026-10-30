#pragma once

#include <string>

#include "numathap/backend/limit/limit.hpp"
#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"

namespace numathap::backend::limit::internal {

enum class Approach {
    Bilateral,
    FromRight,
    FromLeft
};

LimitResult calculateAlgebraicSingularity(
    const math::PreparedAst& normalized,
    const std::string& variable,
    const core::Value& pointValue,
    Approach approach,
    core::Context& context);

}  // namespace numathap::backend::limit::internal