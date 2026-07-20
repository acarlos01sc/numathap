/**
 * @file Capability.hpp
 * @brief Defines the available optional capabilities for the processing
 * pipeline.
 */
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
    /**
     * @brief Enables the expression simplifier to optimize the AST.
     *
     * When active, the simplifier reduces the complexity of mathematical
     * expressions by applying algebraic identities and constant folding.
     */
    Simplifier
};

}  // namespace numathap::config