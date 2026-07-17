#include "numathap/math/PreparedAst.hpp"

#include "numathap/math/MathAstPrinter.hpp"
#include "numathap/orchestration/Orchestrator.hpp"

namespace numathap::math {

PreparedAst::PreparedAst(const MathAst& mathAst,
                         const config::MathEnvironment& environment)
    : expression_(mathAst.expression()), environment_(environment) {
    orchestration::Orchestrator orchestrator;

    root_ = orchestrator.build(mathAst, environment);
}

const MathNode* PreparedAst::root() const noexcept { return root_.get(); }

const std::string& PreparedAst::expression() const noexcept {
    return expression_;
}

bool PreparedAst::empty() const noexcept { return root_ == nullptr; }

const config::MathEnvironment& PreparedAst::environment() const noexcept {
    return environment_;
}

void PreparedAst::print(std::ostream& os) const {
    if (root_) {
        MathAstPrinter printer;
        printer.print(*root_, os);
    }
}

}  // namespace numathap::math