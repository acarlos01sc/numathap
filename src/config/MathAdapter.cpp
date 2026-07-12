#include "numathap/config/MathAdapter.hpp"

namespace numathap::config {

// MathAdapter is a pure abstract interface.
//
// Concrete implementations include:
//
//   - CMathAdapter
//   - BoostMathAdapter
//
// Each adapter is responsible for mapping mathematical functions
// and constants provided by its corresponding numerical library.
// MathAdapter implementations do not perform parsing or name resolution.
// They simply execute mathematical functions and provide numerical
// constants requested by the evaluator.

}  // namespace numathap::config