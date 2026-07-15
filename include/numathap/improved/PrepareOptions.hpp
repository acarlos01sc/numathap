#pragma once

namespace numathap::improved {

/**
 * @brief Options used during expression preparation.
 *
 * PrepareOptions controls how a mathematical expression is transformed
 * into a PreparedExpression.
 *
 * A default-constructed PrepareOptions object enables the library's
 * standard preparation pipeline.
 */
class PrepareOptions {
   public:
    /**
     * @brief Constructs PrepareOptions with the default preparation pipeline.
     */
    PrepareOptions() = default;

    /**
     * @brief Enables or disables expression simplification.
     *
     * Enabled by default.
     */
    PrepareOptions& simplify(bool enable = true);

    /**
     * @brief Returns whether simplification is enabled.
     */
    [[nodiscard]]
    bool simplify() const;

   private:
    bool simplify_ = true;
};

}  // namespace numathap::improved