#pragma once

#include <memory>
#include <string_view>

#include "numathap/improved/EvaluateOptions.hpp"
#include "numathap/improved/IntegrateOptions.hpp"
#include "numathap/improved/LimitOptions.hpp"
#include "numathap/improved/Value.hpp"
#include "numathap/improved/Variables.hpp"

namespace numathap::improved {

/**
 * @brief A prepared mathematical expression.
 *
 * A PreparedExpression owns an internal Prepared-AST and provides
 * mathematical operations such as evaluation, limits and integration.
 *
 * Instances are created exclusively by Session::prepare().
 */
class PreparedExpression {
   public:
    ~PreparedExpression();

    // Non-copyable
    PreparedExpression(const PreparedExpression&) = delete;
    PreparedExpression& operator=(const PreparedExpression&) = delete;

    // Movable
    PreparedExpression(PreparedExpression&&) noexcept;
    PreparedExpression& operator=(PreparedExpression&&) noexcept;

    /**
     * @brief Evaluates the expression.
     *
     * @param variables Variable assignments.
     * @param options Evaluation options.
     * @return Result of the evaluation.
     */
    [[nodiscard]]
    Value calc(const Variables& variables,
               const EvaluateOptions& options = {}) const;

    /**
     * @brief Computes the limit with respect to a variable.
     *
     * @param variable Variable whose limit is taken.
     * @param variables Variable assignments.
     * @param options Limit options.
     * @return Computed limit.
     */
    [[nodiscard]]
    Value limit(std::string_view variable, const Variables& variables,
                const LimitOptions& options = {}) const;

    /**
     * @brief Computes a definite integral.
     *
     * The integration interval must be supplied through Variables.
     *
     * @param variable Integration variable.
     * @param variables Variable assignments and interval.
     * @param options Integration options.
     * @return Integral value.
     */
    [[nodiscard]]
    Value integrate(std::string_view variable, const Variables& variables,
                    const IntegrateOptions& options = {}) const;

   private:
    friend class Session;

    class Impl;

    explicit PreparedExpression(std::unique_ptr<Impl> impl);

    std::unique_ptr<Impl> impl_;
};

}  // namespace numathap::improved