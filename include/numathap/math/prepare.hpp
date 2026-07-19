/**
 * @file prepare.hpp
 * @brief High-level interface for preparing mathematical expressions.
 *
 * This file provides convenient entry points to transform raw mathematical
 * expressions into a structured, semantic-ready PreparedAst.
 */
#pragma once

#include <string>

#include "numathap/config/MathEnvironment.hpp"
#include "numathap/math/PreparedAst.hpp"

namespace numathap::math {

/**
 * @brief Prepares a mathematical expression using a specific environment.
 *
 * This is the primary function for processing expressions. It parses the
 * input string and applies the capabilities defined in the provided
 * {@link numathap::config::MathEnvironment MathEnvironment}.
 *
 * @param expression The mathematical expression string (e.g., "sin(x)").
 * @param environment The environment configuration to use for preparation.
 * @return A PreparedAst object ready for evaluation.
 */
PreparedAst prepare(const std::string& expression,
                    const config::MathEnvironment& environment);

/**
 * @brief Prepares a mathematical expression using the default environment.
 *
 * An overloaded helper that uses a default configuration for environments
 * where explicit customization is not required.
 *
 * @param expression The mathematical expression string.
 * @return A PreparedAst object ready for evaluation.
 */
PreparedAst prepare(const std::string& expression);

}  // namespace numathap::math