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
     * @brief Enables constant folding on the Math-AST.
     */
    ConstantFolder,

    /**
     * @brief Enables algebraic simplification of the Math-AST.
     */
    Simplify,

    /**
     * @brief Enables advanced algebraic simplification of the Math-AST.
     */
    UltraSimplifier
};

}  // namespace numathap::config