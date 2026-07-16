#pragma once

#include <string>

#include "numathap/math/PreparedAst.hpp"
#include "numathap/config/MathEnvironment.hpp"

namespace numathap::math {

PreparedAst prepare(
    const std::string& expression,
    const config::MathEnvironment& environment
);

}