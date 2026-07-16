#include "numathap/math/PreparedAst.hpp"

#include "numathap/math/MathAstPrinter.hpp"
#include "numathap/orchestration/Orchestrator.hpp"

namespace numathap::math {

PreparedAst::PreparedAst(const MathAst& mathAst,
                         const config::MathEnvironment& environment)
    : expression_(mathAst.expression()) {
    orchestration::Orchestrator orchestrator;

    root_ = orchestrator.build(mathAst, environment);
}

const MathNode* PreparedAst::root() const noexcept { return root_.get(); }

const std::string& PreparedAst::expression() const noexcept {
    return expression_;
}

bool PreparedAst::empty() const noexcept { return root_ == nullptr; }

void PreparedAst::print(std::ostream& os) const {
    if (root_) {
        MathAstPrinter printer;
        printer.print(*root_, os);
    }
}

}  // namespace numathap::math