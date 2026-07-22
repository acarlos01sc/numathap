#pragma once

#include "numathap/backend/evaluate.hpp"
#include "numathap/config/MathEnvironment.hpp"
#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"
#include "numathap/math/PreparedAst.hpp"
#include "numathap/math/prepare.hpp"
#include "numathap/config/configure.hpp"

namespace numathap {

using backend::evaluate;
using math::prepare;
using config::configure;
using PreparedAst = math::PreparedAst;
using MathEnvironment = config::MathEnvironment;
using Context = core::Context;
using Value = core::Value;
using MathLibrary = config::MathLibrary;
using NumericType = config::NumericType;

}  // namespace numathap