#include "numathap/Session.hpp"

#include <stdexcept>

namespace numathap {

Session::Session() : environment_{}, configurator_{environment_} {}

config::Configurator& Session::configurator() noexcept { return configurator_; }

const config::Configurator& Session::configurator() const noexcept {
    return configurator_;
}

core::Value Session::evaluate(const std::string& expression,
                              const core::Context& context) const {
    (void)expression;
    (void)context;

    throw std::logic_error("Session::evaluate() is not implemented yet.");
}

const config::MathEnvironment& Session::environment() const noexcept {
    return environment_;
}

}  // namespace numathap