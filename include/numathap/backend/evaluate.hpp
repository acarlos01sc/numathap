#pragma once

#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"
#include "numathap/math/PreparedAst.hpp"

namespace numathap::backend {

[[nodiscard]]
core::Value evaluate(
    const math::PreparedAst& expression,
    const core::Context& context);

} // namespace numathap::backend