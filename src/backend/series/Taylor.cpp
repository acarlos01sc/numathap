/**
 * @file Taylor.cpp
 * @brief Implements the Taylor Series backend.
 */

#include "numathap/backend/series/Taylor.hpp"

#include <stdexcept>

#include "numathap/math/PreparedAst.hpp"

namespace numathap::backend::series {

math::PreparedAst Taylor::series(const math::PreparedAst& prepared,
                                 const std::string& variable,
                                 const core::Value& center,
                                 const TaylorConfig& config) {
    if (config.order == 0) {
        throw std::invalid_argument(
            "Taylor Series order must be greater than zero.");
    }

    (void)prepared;
    (void)variable;
    (void)center;

    throw std::logic_error(
        "Taylor Series is not implemented yet.");
}

}  // namespace numathap::backend::series