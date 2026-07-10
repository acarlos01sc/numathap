#pragma once

namespace numathap::config {

/**
 * @brief Represents an optional mathematical capability.
 *
 * Capabilities extend the processing pipeline by enabling additional
 * transformations on the Math-AST before execution.
 *
 * The availability of each capability is controlled by the Configurator.
 */
enum class Capability {
    Simplifier
};

}  // namespace numathap::config