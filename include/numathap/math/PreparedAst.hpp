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
     * @brief Returns the root node.
     */
    [[nodiscard]]
    const MathNode* root() const noexcept;

    /**
     * @brief Returns the original expression.
     */
    [[nodiscard]]
    const std::string& expression() const noexcept;

    /**
     * @brief Checks whether the tree is empty.
     */
    [[nodiscard]]
    bool empty() const noexcept;

     /**
     * @brief Returns the execution environment used to prepare this AST.
     */
    [[nodiscard]]
    const config::MathEnvironment& environment() const noexcept;

    /**
     * @brief Prints the Prepared-AST.
     */
    void print(std::ostream& os) const;

   private:
    std::string expression_;

    MathNodePtr root_;

    const config::MathEnvironment& environment_;
};

}  // namespace numathap::math