#include <gtest/gtest.h>

#include <numathap/numathap.hpp>

#include <chrono>
#include <cmath>
#include <cerrno>
#include <thread>
#include <stdexcept>

#include <csignal>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace {

constexpr auto kEvaluationTimeout = std::chrono::seconds(60);

// Executado exclusivamente pelo processo filho.
// Código de saída:
//   0 = avaliação correta
//   1 = resultado incorreto
//   2 = exceção durante a avaliação
void evaluateExpression()
{
    try {
        auto ast = numathap::prepare("x^2 + 2*x + 1 + cos(pi)");

        numathap::Context context;
        context.setValue("x", "2");

        const auto result = numathap::evaluate(ast, context);
        const double value = result.real().value();

        if (std::abs(value - 8.0) < 1e-12) {
            _exit(0);
        }

        _exit(1);
    }
    catch (...) {
        _exit(2);
    }
}

}  // namespace

TEST(Evaluate, ComputesExpressionWithinOneMinute)
{
    const pid_t child = fork();

    ASSERT_NE(child, -1)
        << "Não foi possível criar o processo filho.";

    if (child == 0) {
        evaluateExpression();
    }

    const auto deadline =
        std::chrono::steady_clock::now() + kEvaluationTimeout;

    int status = 0;

    while (std::chrono::steady_clock::now() < deadline) {
        const pid_t result = waitpid(child, &status, WNOHANG);

        if (result == child) {
            ASSERT_TRUE(WIFEXITED(status))
                << "O processo filho terminou de forma anormal.";

            ASSERT_EQ(WEXITSTATUS(status), 0)
                << "A avaliação falhou ou produziu um resultado incorreto.";

            SUCCEED();
            return;
        }

        if (result == -1) {
            if (errno == EINTR) {
                continue;
            }

            FAIL() << "Erro ao aguardar o processo filho.";
            return;
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(10)
        );
    }

    // O prazo expirou: interrompe e recolhe o processo filho.
    if (kill(child, SIGKILL) == -1 && errno != ESRCH) {
        FAIL() << "O timeout expirou e não foi possível encerrar "
                  "o processo filho.";
        return;
    }

    while (waitpid(child, &status, 0) == -1 && errno == EINTR) {
    }

    FAIL() << "A avaliação não terminou dentro de 60 segundos.";
}

TEST(Evaluate, MultipleVariables)
{
    auto ast = numathap::prepare("2*x + y + z");

    numathap::Context context;
    context.setValue("x", "3");
    context.setValue("y", "4");
    context.setValue("z", "5");

    const auto result = numathap::evaluate(ast, context);
    const double value = result.real().value();

    EXPECT_NEAR(value, 15.0, 1e-12);
}

TEST(EvaluateErrors, UndefinedVariable)
{
    auto ast = numathap::prepare("2*x + y + z");

    numathap::Context context;
    context.setValue("x", "3");
    context.setValue("y", "4");

    EXPECT_THROW(
        {
            auto result = numathap::evaluate(ast, context);
            (void)result;
        },
        std::invalid_argument
    );
}

TEST(Evaluate, CustomMathEnvironment)
{
    const auto environment = numathap::configure(
        numathap::config::Capability::ConstantFolder,
        numathap::config::Capability::Simplify
    );

    auto ast = numathap::prepare(
        "x + (2 + 3) + 0",
        environment
    );

    numathap::Context context;
    context.setValue("x", "7");

    const auto result = numathap::evaluate(ast, context);

    EXPECT_NEAR(result.real().value(), 12.0, 1e-12);
}

TEST(Evaluate, DivisionAndLogarithm)
{
    auto ast = numathap::prepare("(x + y) / 2 + log(z)");

    numathap::Context context;
    context.setValue("x", "4");
    context.setValue("y", "6");
    context.setValue("z", "1");

    const auto result = numathap::evaluate(ast, context);

    EXPECT_NEAR(result.real().value(), 5.0, 1e-12);
}