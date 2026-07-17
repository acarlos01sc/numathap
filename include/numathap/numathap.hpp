#pragma once

#include "numathap/math/prepare.hpp"
#include "numathap/math/PreparedAst.hpp"
#include "numathap/config/MathEnvironment.hpp"
#include "numathap/core/Context.hpp"
#include "numathap/backend/evaluate.hpp"

namespace numathap {

    using math::prepare;
    using backend::evaluate;
    using PreparedAst = math::PreparedAst;
    using MathEnvironment = config::MathEnvironment;
    using Context = core::Context;

}