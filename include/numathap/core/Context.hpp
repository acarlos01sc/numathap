#pragma once

#include <string>
#include <unordered_map>

#include "numathap/core/Value.hpp"

namespace numathap::core {

class Context {
   public:
    void set(const std::string& name, const Value& value);

    [[nodiscard]]
    bool contains(const std::string& name) const noexcept;

    [[nodiscard]]
    const Value& get(const std::string& name) const;

   private:
    std::unordered_map<std::string, Value> variables_;
};

}  // namespace numathap::core