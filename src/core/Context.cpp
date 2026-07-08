#include "numathap/core/Context.hpp"

#include <stdexcept>

namespace numathap::core {

void Context::set(const std::string& name,
                  const Value& value) {
    variables_[name] = value;
}

bool Context::contains(const std::string& name) const noexcept {
    return variables_.find(name) != variables_.end();
}

const Value& Context::get(const std::string& name) const {
    auto it = variables_.find(name);

    if (it == variables_.end()) {
        throw std::runtime_error(
            "Undefined variable: " + name);
    }

    return it->second;
}

}  // namespace numathap::core