#pragma once

#include "numathap/core/Context.hpp"

namespace numathap {

/**
 * @brief Public evaluation context.
 *
 * This class is the public interface used by applications.
 * It wraps the internal core::Context.
 */
class Context {
   public:
    Context() = default;

    [[nodiscard]]
    core::Context& core() noexcept;

    [[nodiscard]]
    const core::Context& core() const noexcept;

    [[nodiscard]]
    const core::Value& get(const std::string& name) const;

    void set(const std::string& name, const core::Value& value);

   private:
    core::Context context_;
};

}  // namespace numathap