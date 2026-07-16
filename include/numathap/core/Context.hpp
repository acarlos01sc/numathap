#pragma once

#include <optional>
#include <string>
#include <unordered_map>

namespace numathap::core {

class Context {
public:

    struct Interval {
        std::string lower;
        std::string upper;
    };

    Context() = default;

    // ------------------------------------------------------------
    // Variable definitions
    // ------------------------------------------------------------

    void setValue(const std::string& symbol,
                  const std::string& definition);

    [[nodiscard]]
    std::optional<std::string>
    findValue(const std::string& symbol) const;

    [[nodiscard]]
    bool hasValue(const std::string& symbol) const;

    // ------------------------------------------------------------
    // Interval definitions
    // ------------------------------------------------------------

    void setInterval(const std::string& symbol,
                     const std::string& lower,
                     const std::string& upper);

    [[nodiscard]]
    std::optional<Interval>
    findInterval(const std::string& symbol) const;

    [[nodiscard]]
    bool hasInterval(const std::string& symbol) const;

    // ------------------------------------------------------------

    void clear();

private:

    std::unordered_map<std::string, std::string> values_;

    std::unordered_map<std::string, Interval> intervals_;
};

} // namespace numathap::core