/**
 * @file PreparedAst.hpp
 * @brief Definition of the PreparedAst class.
 *
 * This file provides the structure for an AST that has been processed
 * and prepared for execution within a specific mathematical environment.
 */
#pragma once

#include <ostream>
#include <string>

#include "numathap/config/MathEnvironment.hpp"
#include "numathap/math/MathAst.hpp"
#include "numathap/math/MathNode.hpp"

namespace numathap::math {

/**
 * @brief Prepared mathematical Abstract Syntax Tree.
 *
 * A PreparedAst is generated from a MathAst using a configured
 * MathEnvironment. During construction, the Orchestrator applies the
 * capabilities enabled in the environment, producing a tree ready for
 * execution by the Dispatcher.
 *
 * Multiple PreparedAst objects may be created from the same MathAst,
 * each using a different MathEnvironment.
 */
class PreparedAst {
   public:
    /**
     * @brief Builds a PreparedAst from a MathAst.
     *
     * @param mathAst Source Math-AST.
     * @param environment Execution environment.
     */
    PreparedAst(const MathAst& mathAst,
                const config::MathEnvironment& environment);

    PreparedAst(PreparedAst&&) noexcept = default;
    PreparedAst& operator=(PreparedAst&&) noexcept = default;

    PreparedAst(const PreparedAst&) = delete;
    PreparedAst& operator=(const PreparedAst&) = delete;

    ~PreparedAst() = default;

    /**
     * @brief Gets the root node of the prepared AST.
     * @return A pointer to the root MathNode.
     */
    [[nodiscard]]
    const MathNode* root() const noexcept;

    /**
     * @brief Gets the original mathematical expression string.
     * @return The original expression.
     */
    [[nodiscard]]
    const std::string& expression() const noexcept;

    /**
     * @brief Checks whether the prepared AST is empty.
     * @return true if the AST contains no nodes, false otherwise.
     */
    [[nodiscard]]
    bool empty() const noexcept;

    /**
     * @brief Gets the execution environment used to prepare this AST.
     * @return A const reference to the MathEnvironment.
     */
    [[nodiscard]]
    const config::MathEnvironment& environment() const noexcept;

    /**
     * @brief Prints the Prepared-AST to an output stream.
     * @param os The output stream to write to.
     */
    void print(std::ostream& os) const;

   private:
    /// @brief The original expression string.
    std::string expression_;

    /// @brief The root node of the AST.
    MathNodePtr root_;

    /// @brief The environment used during the preparation phase.
    config::MathEnvironment environment_;
};

}  // namespace numathap::math