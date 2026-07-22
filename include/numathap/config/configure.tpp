#pragma once

namespace numathap::config {

template <typename... Options>
MathEnvironment configure(const Options&... options) {
    MathEnvironment environment;

    Configurator configurator(environment);

    (configurator.apply(options), ...);

    return environment;
}

}  // namespace numathap::config