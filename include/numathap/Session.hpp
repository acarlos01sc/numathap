#pragma once

#include <memory>
#include <string>

#include "numathap/config/Configurator.hpp"
#include "numathap/config/MathEnvironment.hpp"
#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"

namespace numathap {

namespace backend {
class Evaluator;
}

namespace math {
class MathNode;
using MathNodePtr = std::unique_ptr<MathNode>;
}  // namespace math

class Session {
   public:
    /**
     * @brief Represents a prepared mathematical expression.
     *
     * A PreparedExpression contains a prepared AST and can be
     * evaluated repeatedly with different contexts.
     */
    class PreparedExpression {
       public:
        PreparedExpression(std::unique_ptr<backend::Evaluator> evaluator);

        ~PreparedExpression();

        [[nodiscard]]
        core::Value calc(const core::Context& context = {}) const;

        [[nodiscard]]
        core::Value operator()(const core::Context& context = {}) const;

       private:
        std::unique_ptr<backend::Evaluator> evaluator_;
    };

   public:
    Session();

    /**
     * @brief Evaluates an expression immediately.
     *
     * This is a convenience method equivalent to:
     *
     * prepare(expression).calc(context)
     */
    [[nodiscard]]
    core::Value evaluate(const std::string& expression,
                         const core::Context& context = {}) const;

    /**
     * @brief Prepares an expression for repeated evaluation.
     */
    [[nodiscard]]
    PreparedExpression prepare(const std::string& expression) const;

    /**
     * @brief Returns the configuration interface.
     */
    config::Configurator& configurator() noexcept;

    /**
     * @brief Returns the configuration interface.
     */
    [[nodiscard]]
    const config::Configurator& configurator() const noexcept;

    [[nodiscard]]
    const config::MathEnvironment& environment() const noexcept;

   private:
    config::MathEnvironment environment_;

    config::Configurator configurator_;
};

}  // namespace numathap