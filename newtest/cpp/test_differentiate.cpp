#include <gtest/gtest.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cerrno>
#include <chrono>
#include <csignal>
#include <thread>

#include "numathap/numathap.hpp"

namespace {

using namespace numathap;

constexpr auto kTestTimeout = std::chrono::seconds(60);
constexpr auto kTolerance = 1e-8;

template <typename Expected>
void runDifferentiateTest(const char* expressionText,
                          const char* variable,
                          const char* variableValue,
                          Expected expectedResult) {
    const auto expression = prepare(expressionText);
    const auto derivative = differentiate(expression, variable);

    Context context;
    context.setValue(variable, variableValue);

    const Value result = evaluate(derivative, context);

    if (!result.isReal()) {
        _exit(1);
    }

    const auto actualResult = result.real().value();

    if (actualResult < expectedResult - kTolerance ||
        actualResult > expectedResult + kTolerance) {
        _exit(1);
    }

    _exit(0);
}

template <typename Expected>
void runTestWithTimeout(const char* expressionText,
                        const char* variable,
                        const char* variableValue,
                        Expected expectedResult) {
    const pid_t child = fork();

    ASSERT_NE(child, -1) << "Failed to create child process.";

    if (child == 0) {
        try {
            runDifferentiateTest(
                expressionText,
                variable,
                variableValue,
                expectedResult);
        } catch (...) {
            _exit(2);
        }
    }

    const auto deadline =
        std::chrono::steady_clock::now() + kTestTimeout;

    int status = 0;

    while (std::chrono::steady_clock::now() < deadline) {
        const pid_t result = waitpid(child, &status, WNOHANG);

        if (result == child) {
            ASSERT_TRUE(WIFEXITED(status))
                << "Child process terminated abnormally.";

            ASSERT_EQ(WEXITSTATUS(status), 0)
                << "Symbolic differentiation test failed or threw an "
                   "exception.";

            return;
        }

        if (result == -1) {
            if (errno == EINTR) {
                continue;
            }

            FAIL() << "waitpid failed.";
            return;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    const int killResult = kill(child, SIGKILL);

    if (killResult == -1 && errno != ESRCH) {
        FAIL() << "Failed to terminate child process.";
        return;
    }

    while (waitpid(child, &status, 0) == -1) {
        if (errno == EINTR) {
            continue;
        }

        FAIL() << "Failed to wait for child process termination.";
        return;
    }

    FAIL() << "Test exceeded the maximum execution time of 60 seconds.";
}

TEST(NumathapDifferentiate, Polynomial) {
    runTestWithTimeout("x^3", "x", "2", 12.0);
}

TEST(NumathapDifferentiate, PolynomialSum) {
    runTestWithTimeout(
        "x^3+2*x^2-5*x+7",
        "x",
        "2",
        15.0);
}

TEST(NumathapDifferentiate, Sine) {
    runTestWithTimeout("sin(x)", "x", "0", 1.0);
}

TEST(NumathapDifferentiate, ChainRule) {
    runTestWithTimeout("sin(2*x)", "x", "0", 2.0);
}

TEST(NumathapDifferentiate, Reciprocal) {
    runTestWithTimeout("1/x", "x", "2", -0.25);
}

TEST(NumathapDifferentiate, Exponential) {
    runTestWithTimeout("exp(x)", "x", "0", 1.0);
}

TEST(NumathapDifferentiate, Logarithm) {
    runTestWithTimeout("log(x)", "x", "1", 1.0);
}

TEST(NumathapDifferentiate, ProductRule) {
    runTestWithTimeout("x^2*sin(x)", "x", "0", 0.0);
}

}  // namespace