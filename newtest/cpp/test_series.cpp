#include <gtest/gtest.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cerrno>
#include <chrono>
#include <csignal>
#include <cstddef>
#include <exception>
#include <iostream>
#include <thread>

#include "numathap/numathap.hpp"

namespace {

using namespace numathap;

constexpr auto kTestTimeout = std::chrono::seconds(60);

constexpr auto kTolerance = 1e-8;

void runTaylorTest(const char* expressionText, std::size_t order,
                   auto expectedResult) {
    const auto expression = prepare(expressionText);

    const auto environment =
        configure(backend::series::TaylorConfig{.order = order});

    const auto expansion = series(expression, "x", "0", environment);

    Context context;
    context.setValue("x", "0.1");

    const Value result = evaluate(expansion, context);

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

void runTestWithTimeout(const char* expressionText, std::size_t order,
                        auto expectedResult) {
    const pid_t child = fork();

    ASSERT_NE(child, -1) << "Failed to create child process.";

    if (child == 0) {
        //        try {
        //            runTaylorTest(expressionText, order, expectedResult);
        //        } catch (...) {
        //            _exit(2);
        //        }
        try {
            runTaylorTest(expressionText, order, expectedResult);
        } catch (const std::exception& e) {
            std::cerr << "Taylor test failed for expression '" << expressionText
                      << "': " << e.what() << '\n';
            _exit(2);
        } catch (...) {
            std::cerr << "Taylor test failed with an unknown exception for "
                         "expression '"
                      << expressionText << "'.\n";
            _exit(2);
        }
    }

    const auto deadline = std::chrono::steady_clock::now() + kTestTimeout;

    int status = 0;

    while (std::chrono::steady_clock::now() < deadline) {
        const pid_t result = waitpid(child, &status, WNOHANG);

        if (result == child) {
            ASSERT_TRUE(WIFEXITED(status))
                << "Child process terminated abnormally.";

            ASSERT_EQ(WEXITSTATUS(status), 0)
                << "Taylor series test failed or threw an exception.";

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

TEST(NumathapSeries, TaylorSin10Terms) {
    runTestWithTimeout("sin(x)", 10, 0.09983341664682815);
}

TEST(NumathapSeries, TaylorSin20Terms) {
    runTestWithTimeout("sin(x)", 20, 0.09983341664682815);
}

TEST(NumathapSeries, TaylorXSqrt10Terms) {
    runTestWithTimeout("x/sqrt(1-x^2)", 10, 0.10050378152592121);
}

TEST(NumathapSeries, TaylorXSqrt20Terms) {
    runTestWithTimeout("x/sqrt(1-x^2)", 20, 0.10050378152592121);
}

TEST(NumathapSeries, TaylorRationalTrig10Terms) {
    runTestWithTimeout("1/(1+cos(x)+sin(x))", 10, 0.47736397734527186);
}

TEST(NumathapSeries, TaylorRationalTrig20Terms) {
    runTestWithTimeout("1/(1+cos(x)+sin(x))", 20, 0.47736397734527186);
}

TEST(NumathapSeries, TaylorAsin10Terms) {
    runTestWithTimeout("asin(x)", 10, 0.1001674211615598);
}

TEST(NumathapSeries, TaylorAsin20Terms) {
    runTestWithTimeout("asin(x)", 20, 0.1001674211615598);
}

TEST(NumathapSeries, TaylorAtan10Terms) {
    runTestWithTimeout("atan(x)", 10, 0.09966865249116204);
}

TEST(NumathapSeries, TaylorAtan20Terms) {
    runTestWithTimeout("atan(x)", 20, 0.09966865249116204);
}

TEST(NumathapSeries, TaylorAcos10Terms) {
    runTestWithTimeout("acos(x)", 10, 1.4706289056333368);
}

TEST(NumathapSeries, TaylorAcos20Terms) {
    runTestWithTimeout("acos(x)", 20, 1.4706289056333368);
}

}  // namespace