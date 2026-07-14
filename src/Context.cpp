#include "numathap/Context.hpp"

namespace numathap {

core::Context& Context::core() noexcept { return context_; }

const core::Context& Context::core() const noexcept { return context_; }

const core::Value& Context::get(const std::string& name) const {
    return context_.get(name);
}

void Context::set(const std::string& name, const core::Value& value) {
    context_.set(name, value);
}

}  // namespace numathap