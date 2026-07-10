#include "numathap/orchestration/Orchestrator.hpp"

namespace numathap::orchestration {

math::MathNodePtr Orchestrator::prepare(
    math::MathNodePtr ast,
    const config::MathEnvironment& environment) const {
    //----------------------------------------------------------
    // Future preparation stages:
    //
    // - symbolic simplification
    // - constant folding
    // - normalization
    // - capability-specific transformations
    //
    // The enabled capabilities are obtained from the execution
    // environment.
    //----------------------------------------------------------

    (void)environment;

    return ast;
}

}  // namespace numathap::orchestration