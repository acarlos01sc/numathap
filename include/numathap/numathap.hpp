#pragma once

#include "numathap/backend/differentiate/differentiate.hpp"
#include "numathap/backend/evaluate.hpp"
#include "numathap/backend/evaluate_cx.hpp"
#include "numathap/backend/integrate/integrate.hpp"
#include "numathap/backend/series/series.hpp"
#include "numathap/config/CMathDoubleAdapter.hpp"
#include "numathap/config/ComplexDoubleAdapter.hpp"
#include "numathap/config/MathEnvironment.hpp"
#include "numathap/config/configure.hpp"
#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"
#include "numathap/math/PreparedAst.hpp"
#include "numathap/math/prepare.hpp"

namespace numathap {

using backend::evaluate;
using backend::evaluate_cx;
using backend::differentiate::differentiate;
using backend::integrate::integrate;
using backend::series::series;
using config::configure;
using math::prepare;
using PreparedAst = math::PreparedAst;
using MathEnvironment = config::MathEnvironment;
using CMathDoubleAdapter = config::CMathDoubleAdapter;
using ComplexDoubleAdapter = config::ComplexDoubleAdapter;
using Context = core::Context;
using Value = core::Value;
using MathLibrary = config::MathLibrary;
using NumericType = config::NumericType;

}  // namespace numathap