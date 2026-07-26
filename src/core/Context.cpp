#include "numathap/core/Context.hpp"

#include <stdexcept>
#include <unordered_set>

namespace numathap::core {

//------------------------------------------------------------
// Variables
//------------------------------------------------------------

void Context::setValue(const std::string& symbol,
                       const std::string& definition) {
    values_[symbol] = definition;
}

std::optional<std::string> Context::findValue(const std::string& symbol) const {
    auto it = values_.find(symbol);

    if (it == values_.end()) return std::nullopt;

    return it->second;
}

std::string Context::resolveValue(const std::string& expression) const {
    std::unordered_set<std::string> visited;
    return resolveValue(expression, visited);
}

std::string Context::resolveValue(
    const std::string& expression,
    std::unordered_set<std::string>& visited) const {
    auto value = findValue(expression);

    if (!value) {
        return expression;
    }

    if (!visited.insert(expression).second) {
        throw std::runtime_error(
            "Circular value definition involving symbol '" + expression + "'.");
    }

    return resolveValue(*value, visited);
}

bool Context::hasValue(const std::string& symbol) const {
    return values_.find(symbol) != values_.end();
}

//------------------------------------------------------------
// Intervals
//------------------------------------------------------------

void Context::setInterval(const std::string& symbol, const std::string& lower,
                          const std::string& upper) {
    intervals_[symbol] = Interval{lower, upper};
}

std::optional<Context::Interval> Context::findInterval(
    const std::string& symbol) const {
    auto it = intervals_.find(symbol);

    if (it == intervals_.end()) return std::nullopt;

    return it->second;
}

bool Context::hasInterval(const std::string& symbol) const {
    return intervals_.find(symbol) != intervals_.end();
}

//------------------------------------------------------------

void Context::clear() {
    values_.clear();
    intervals_.clear();
}

}  // namespace numathap::core