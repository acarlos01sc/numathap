#include "numathap/core/Context.hpp"

namespace numathap::core {

//------------------------------------------------------------
// Variables
//------------------------------------------------------------

void Context::setValue(const std::string& symbol,
                       const std::string& definition)
{
    values_[symbol] = definition;
}

std::optional<std::string>
Context::findValue(const std::string& symbol) const
{
    auto it = values_.find(symbol);

    if (it == values_.end())
        return std::nullopt;

    return it->second;
}

bool Context::hasValue(const std::string& symbol) const
{
    return values_.find(symbol) != values_.end();
}

//------------------------------------------------------------
// Intervals
//------------------------------------------------------------

void Context::setInterval(const std::string& symbol,
                          const std::string& lower,
                          const std::string& upper)
{
    intervals_[symbol] = Interval{lower, upper};
}

std::optional<Context::Interval>
Context::findInterval(const std::string& symbol) const
{
    auto it = intervals_.find(symbol);

    if (it == intervals_.end())
        return std::nullopt;

    return it->second;
}

bool Context::hasInterval(const std::string& symbol) const
{
    return intervals_.find(symbol) != intervals_.end();
}

//------------------------------------------------------------

void Context::clear()
{
    values_.clear();
    intervals_.clear();
}

} // namespace numathap::core