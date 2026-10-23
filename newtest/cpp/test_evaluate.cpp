#include <gtest/gtest.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cerrno>
#include <chrono>
#include <csignal>
#include <numathap/numathap.hpp>
#include <stdexcept>
#include <thread>

namespace {

constexpr auto kEvaluationTimeout = std::chrono::seconds(60);

// Executed exclusively by the child process
// Output code:
//   0 = Passed
//   1 = Error
//   2 = Exception
void evaluateExpression() {
    try {
        auto ast = numathap::prepare("x^2 + 2*x + 1 + cos(pi)");

        numathap::Context context;
        context.setValue("x", "2");

        const auto result = numathap::evaluate(ast, context);
        const auto value = result.real().value();

        if (value >= 8.0 - 1e-12 && value <= 8.0 + 1e-12) {
            _exit(0);
        }

        _exit(1);
    } catch (...) {
        _exit(2);
    }
}

}  // namespace

TEST(Evaluate, ComputesExpressionWithinOneMinute) {
    const pid_t child = fork();

    ASSERT_NE(child, -1) << "Can't create child process.";

    if (child == 0) {
        evaluateExpression();
    }

    const auto deadline = std::chrono::steady_clock::now() + kEvaluationTimeout;

    int status = 0;

    while (std::chrono::steady_clock::now() < deadline) {
        const pid_t result = waitpid(child, &status, WNOHANG);

        if (result == child) {
            ASSERT_TRUE(WIFEXITED(status))
                << "The child process terminated abnormally.";

            ASSERT_EQ(WEXITSTATUS(status), 0) << "Incorrect result.";

            SUCCEED();
            return;
        }

        if (result == -1) {
            if (errno == EINTR) {
                continue;
            }

            FAIL() << "Error while waiting for the child process.";
            return;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // O prazo expirou: interrompe e recolhe o processo filho.
    if (kill(child, SIGKILL) == -1 && errno != ESRCH) {
        FAIL() << "The timeout expired and it was not possible to terminate "
                  "child process.";
        return;
    }

    while (waitpid(child, &status, 0) == -1 && errno == EINTR) {
    }

    FAIL() << "The evaluation did not finish within 60 seconds.";
}

TEST(Evaluate, MultipleVariables) {
    auto ast = numathap::prepare("2*x + y + z");

    numathap::Context context;
    context.setValue("x", "3");
    context.setValue("y", "4");
    context.setValue("z", "5");

    const auto result = numathap::evaluate(ast, context);
    const auto value = result.real().value();

    EXPECT_NEAR(value, 15.0, 1e-12);
}

TEST(EvaluateErrors, UndefinedVariable) {
    auto ast = numathap::prepare("2*x + y + z");

    numathap::Context context;
    context.setValue("x", "3");
    context.setValue("y", "4");

    EXPECT_THROW(
        {
            auto result = numathap::evaluate(ast, context);
            (void)result;
        },
        std::invalid_argument);
}

TEST(Evaluate, CustomMathEnvironment) {
    const auto environment =
        numathap::configure(numathap::config::Capability::ConstantFolder,
                            numathap::config::Capability::Simplify);

    auto ast = numathap::prepare("x + (2 + 3) + 0", environment);

    numathap::Context context;
    context.setValue("x", "7");

    const auto result = numathap::evaluate(ast, context);

    EXPECT_NEAR(result.real().value(), 12.0, 1e-12);
}

TEST(Evaluate, DivisionAndLogarithm) {
    auto ast = numathap::prepare("(x + y) / 2 + log(z)");

    numathap::Context context;
    context.setValue("x", "4");
    context.setValue("y", "6");
    context.setValue("z", "1");

    const auto result = numathap::evaluate(ast, context);

    EXPECT_NEAR(result.real().value(), 5.0, 1e-12);
}