#pragma once

#include <string>

#include "numathap/config/MathEnvironment.hpp"
#include "numathap/math/PreparedAst.hpp"

namespace numathap::math {

PreparedAst prepare(const std::string& expression,
                    const config::MathEnvironment& environment);

PreparedAst prepare(const std::string& expression);

}  // namespace numathap::math