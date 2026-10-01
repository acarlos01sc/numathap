#include "numathap/backend/Evaluator_cx.hpp"

#include <stdexcept>

#include <vector>

#include "numathap/backend/BackendSupport.hpp"

#include "numathap/config/MathAdapter.hpp"
#include "numathap/dispatch/Dispatcher.hpp"

namespace numathap::backend {

namespace {

const core::Context emptyContext;

} // namespace

Evaluator_cx::Evaluator_cx(
    const math::PreparedAst& prepared,
    const core::Context& context)
    : prepared_(prepared), context_(context), values_(nullptr) {}

Evaluator_cx::Evaluator_cx(
    const math::PreparedAst& prepared,
    const std::unordered_map<std::string, core::Value>& values)
    : prepared_(prepared), context_(emptyContext), values_(&values) {}

core::Value Evaluator_cx::evaluate(
    const math::PreparedAst& prepared,
    const core::Context& context) {

  Evaluator_cx evaluator(prepared, context);

  return dispatch::Dispatcher::dispatch(prepared, evaluator);
}

core::Value Evaluator_cx::evaluateAt(
    const math::PreparedAst& prepared,
    const std::unordered_map<std::string, core::Value>& values) {

  Evaluator_cx evaluator(prepared, values);

  return dispatch::Dispatcher::dispatch(prepared, evaluator);
}

core::Value Evaluator_cx::dispatch(
    const math::MathNode& node) const {

  return dispatch::Dispatcher::dispatch(node, *this);
}

core::Value Evaluator_cx::operator()(
    const math::NumberNode& node) const {

  return BackendSupport::evaluateConstant_cx(
      node.value,
      context_,
      prepared_.environment());
}

core::Value Evaluator_cx::operator()(
    const math::SymbolNode& node) const {

  return resolveSymbol(node.name);
}

core::Value Evaluator_cx::operator()(
    const math::UnaryNode& node) const {

  const auto value = dispatch(*node.operand);

  switch (node.op) {

  case math::UnaryOp::Plus:
    return +value;

  case math::UnaryOp::Minus:
    return -value;
  }

  throw std::logic_error(
      "Evaluator_cx: unknown unary operator.");
}

core::Value Evaluator_cx::operator()(
    const math::BinaryNode& node) const {

  const auto lhs = dispatch(*node.left);
  const auto rhs = dispatch(*node.right);

  switch (node.op) {

  case math::BinaryOp::Add:
    return lhs + rhs;

  case math::BinaryOp::Subtract:
    return lhs - rhs;

  case math::BinaryOp::Multiply:
    return lhs * rhs;

  case math::BinaryOp::Divide:
    return lhs / rhs;

  case math::BinaryOp::Power: {

    std::vector<core::Value> arguments;
    arguments.reserve(2);

    arguments.push_back(lhs);
    arguments.push_back(rhs);

    return prepared_.environment().mathAdapter().callFunction(
        "pow",
        arguments);
  }

  }

  throw std::logic_error(
      "Evaluator_cx: unknown binary operator.");
}

core::Value Evaluator_cx::operator()(
    const math::FunctionNode& node) const {

  std::vector<core::Value> arguments;
  arguments.reserve(node.arguments.size());

  for (const auto& arg : node.arguments) {

    arguments.push_back(dispatch(*arg));
  }

  return prepared_.environment().mathAdapter().callFunction(
      node.name,
      arguments);
}

core::Value Evaluator_cx::resolveSymbol(
    const std::string& symbol) const {

  if (values_) {

    const auto it = values_->find(symbol);

    if (it != values_->end()) {

      return it->second;
    }
  }

  auto definition = context_.findValue(symbol);

  if (!definition) {

    return prepared_.environment().mathAdapter().resolveConstant(
        symbol);
  }

  return BackendSupport::evaluateConstant_cx(
      *definition,
      context_,
      prepared_.environment());
}

} // namespace numathap::backend