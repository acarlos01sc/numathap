#pragma once

#include <chrono>
#include <cerrno>
#include <csignal>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <utility>
#include <vector>

namespace testfw2 {

struct AssertionError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct TestCase {
    std::string suite;
    std::string name;
    std::function<void()> fn;

    std::string fullName() const {
        return suite + "." + name;
    }
};

inline std::vector<TestCase>& registry() {
    static std::vector<TestCase> tests;
    return tests;
}

struct TestRegistrar {
    TestRegistrar(const std::string& suite,
                  const std::string& name,
                  std::function<void()> fn) {
        registry().push_back({suite, name, std::move(fn)});
    }
};

inline constexpr unsigned int defaultTimeoutSeconds = 60;

inline unsigned int& timeoutSeconds() {
    static unsigned int timeout = defaultTimeoutSeconds;
    return timeout;
}

inline void setTimeoutSeconds(unsigned int seconds) {
    timeoutSeconds() = seconds;
}

namespace detail {

inline constexpr int successExitCode = 0;
inline constexpr int failureExitCode = 1;

inline bool writeMessage(int fd, const std::string& message) {
    const char* data = message.data();
    std::size_t remaining = message.size();

    while (remaining > 0) {
        const ssize_t written = ::write(fd, data, remaining);

        if (written < 0) {
            if (errno == EINTR) {
                continue;
            }
            return false;
        }

        data += written;
        remaining -= static_cast<std::size_t>(written);
    }

    return true;
}

inline std::string readMessage(int fd) {
    std::string message;
    char buffer[256];

    while (true) {
        const ssize_t count = ::read(fd, buffer, sizeof(buffer));

        if (count > 0) {
            message.append(buffer, static_cast<std::size_t>(count));
            continue;
        }

        if (count < 0 && errno == EINTR) {
            continue;
        }

        break;
    }

    return message;
}

inline int runTestInChild(const TestCase& test, int pipeFd) {
    try {
        test.fn();
        return successExitCode;
    } catch (const std::exception& e) {
        writeMessage(pipeFd, e.what());
        return failureExitCode;
    } catch (...) {
        writeMessage(pipeFd, "unknown non-standard exception");
        return failureExitCode;
    }
}

struct TestResult {
    bool passed = false;
    bool timedOut = false;
    std::string message;
};

inline TestResult runWithTimeout(const TestCase& test,
                                 unsigned int timeoutSeconds) {
    int pipeFds[2];

    if (::pipe(pipeFds) != 0) {
        return {
            false,
            false,
            "failed to create communication pipe"
        };
    }

    const pid_t pid = ::fork();

    if (pid < 0) {
        ::close(pipeFds[0]);
        ::close(pipeFds[1]);

        return {
            false,
            false,
            "failed to create test process"
        };
    }

    if (pid == 0) {
        // Child process.
        ::close(pipeFds[0]);

        const int result = runTestInChild(test, pipeFds[1]);

        ::close(pipeFds[1]);
        ::_exit(result);
    }

    // Parent process.
    ::close(pipeFds[1]);

    const auto start = std::chrono::steady_clock::now();

    int status = 0;

    while (true) {
        const pid_t result = ::waitpid(pid, &status, WNOHANG);

        if (result == pid) {
            const std::string message = readMessage(pipeFds[0]);
            ::close(pipeFds[0]);

            if (WIFEXITED(status) &&
                WEXITSTATUS(status) == successExitCode) {
                return {true, false, {}};
            }

            return {
                false,
                false,
                message.empty() ? "test failed" : message
            };
        }

        if (result < 0) {
            if (errno == EINTR) {
                continue;
            }

            ::close(pipeFds[0]);

            return {
                false,
                false,
                "waitpid failed"
            };
        }

        const auto now = std::chrono::steady_clock::now();
        const auto elapsed =
            std::chrono::duration_cast<std::chrono::seconds>(
                now - start);

        if (elapsed.count() >= timeoutSeconds) {
            ::kill(pid, SIGKILL);
            ::waitpid(pid, &status, 0);
            ::close(pipeFds[0]);

            return {
                false,
                true,
                "TIMEOUT after " +
                    std::to_string(timeoutSeconds) +
                    " seconds"
            };
        }

        ::usleep(10000);
    }
}

}  // namespace detail

inline int runFiltered(const std::string& filter) {
    int failed = 0;
    int ran = 0;

    for (const auto& test : registry()) {
        if (!filter.empty() &&
            test.fullName().find(filter) == std::string::npos) {
            continue;
        }

        ++ran;

        std::cout << "[ RUN      ] "
                  << test.fullName()
                  << '\n';

        const detail::TestResult result =
            detail::runWithTimeout(test, timeoutSeconds());

        if (result.passed) {
            std::cout << "[       OK ] "
                      << test.fullName()
                      << '\n';
        } else {
            std::cout << "[  FAILED  ] "
                      << test.fullName()
                      << ": "
                      << result.message
                      << '\n';

            ++failed;
        }
    }

    if (ran == 0) {
        std::cout << "No tests matched filter \""
                  << filter
                  << "\"\n";
        return 1;
    }

    std::cout << "\n"
              << (ran - failed)
              << "/"
              << ran
              << " tests passed.\n";

    return failed;
}

inline int runAll() {
    return runFiltered("");
}

}  // namespace testfw2

#define TEST(suite, name)                                             \
    void suite##_##name##_impl();                                    \
    static ::testfw2::TestRegistrar suite##_##name##_registrar(       \
        #suite, #name, suite##_##name##_impl);                       \
    void suite##_##name##_impl()

#define EXPECT_EQ(a, b)                                               \
    do {                                                              \
        if (!((a) == (b))) {                                          \
            std::string message =                                    \
                #a " != " #b " (line " +                             \
                std::to_string(__LINE__) + ")";                      \
            throw ::testfw2::AssertionError(message);                \
        }                                                             \
    } while (0)

#define EXPECT_TRUE(cond)                                             \
    do {                                                              \
        if (!(cond)) {                                                \
            throw ::testfw2::AssertionError(                         \
                #cond " is false (line " +                           \
                std::to_string(__LINE__) + ")");                     \
        }                                                             \
    } while (0)

#define EXPECT_THROWS(expr, exceptionType)                            \
    do {                                                              \
        bool threw = false;                                           \
        try {                                                         \
            (void)(expr);                                             \
        } catch (const exceptionType&) {                              \
            threw = true;                                             \
        }                                                             \
        if (!threw) {                                                 \
            throw ::testfw2::AssertionError(                         \
                #expr " did not throw " #exceptionType               \
                " (line " + std::to_string(__LINE__) + ")");          \
        }                                                             \
    } while (0)