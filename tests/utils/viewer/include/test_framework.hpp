#pragma once

#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// Mini test harness, sem dependências externas.
// Sintaxe (TEST, EXPECT_EQ, EXPECT_TRUE, EXPECT_THROWS) modelada de propósito
// para se parecer com GoogleTest, para facilitar uma migração futura sem
// reescrever o corpo dos testes -- só trocaria este header pelo <gtest/gtest.h>.

namespace testfw {

struct AssertionError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct TestCase {
    std::string suite;
    std::string name;
    std::function<void()> fn;

    std::string fullName() const { return suite + "." + name; }
};

inline std::vector<TestCase>& registry() {
    static std::vector<TestCase> tests;
    return tests;
}

struct TestRegistrar {
    TestRegistrar(const std::string& suite, const std::string& name,
                  std::function<void()> fn) {
        registry().push_back({suite, name, std::move(fn)});
    }
};

// Roda os testes cujo nome completo ("Suite.Name") contém `filter`.
// filter vazio => roda todos. Retorna a quantidade de falhas.
inline int runFiltered(const std::string& filter) {
    int failed = 0;
    int ran = 0;
    for (auto& t : registry()) {
        if (!filter.empty() && t.fullName().find(filter) == std::string::npos) {
            continue;
        }
        ++ran;
        std::cout << "[ RUN      ] " << t.fullName() << '\n';
        try {
            t.fn();
            std::cout << "[       OK ] " << t.fullName() << '\n';
        } catch (const std::exception& e) {
            std::cout << "[  FAILED  ] " << t.fullName() << ": " << e.what()
                       << '\n';
            ++failed;
        }
    }
    if (ran == 0) {
        std::cout << "No tests matched filter \"" << filter << "\"\n";
        return 1;
    }
    std::cout << "\n" << (ran - failed) << "/" << ran << " tests passed.\n";
    return failed;
}

inline int runAll() { return runFiltered(""); }

} // namespace testfw

#define TEST(suite, name)                                                    \
    void suite##_##name##_impl();                                            \
    static ::testfw::TestRegistrar suite##_##name##_registrar(               \
        #suite, #name, suite##_##name##_impl);                               \
    void suite##_##name##_impl()

#define EXPECT_EQ(a, b)                                                      \
    do {                                                                     \
        if (!((a) == (b))) {                                                 \
            std::ostringstream oss;                                          \
            oss << #a " != " #b " (line " << __LINE__ << ")";                \
            throw ::testfw::AssertionError(oss.str());                      \
        }                                                                    \
    } while (0)

#define EXPECT_TRUE(cond)                                                    \
    do {                                                                     \
        if (!(cond)) {                                                      \
            throw ::testfw::AssertionError(#cond " is false (line " +       \
                                            std::to_string(__LINE__) + ")"); \
        }                                                                    \
    } while (0)

#define EXPECT_THROWS(expr, exceptionType)                                   \
    do {                                                                     \
        bool threw = false;                                                 \
        try {                                                               \
            (void)(expr);                                                   \
        } catch (const exceptionType&) {                                    \
            threw = true;                                                   \
        }                                                                   \
        if (!threw) {                                                       \
            throw ::testfw::AssertionError(                                 \
                #expr " did not throw " #exceptionType " (line " +          \
                std::to_string(__LINE__) + ")");                            \
        }                                                                   \
    } while (0)