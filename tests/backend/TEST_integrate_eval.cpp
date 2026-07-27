/**
 * @file TEST_integrate_eval.cpp
 * @brief File based integration pipeline test.
 */

#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "numathap/numathap.hpp"
#include "test_framework.hpp"

namespace {

struct IntegrateCase {
    std::string expression;
    std::string variable;
    std::string lower;
    std::string upper;
    numathap::core::Value expected;
};

std::vector<std::string> split(const std::string& text, char delimiter) {
    std::vector<std::string> result;

    std::stringstream ss(text);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }

    return result;
}

IntegrateCase parseCase(const std::string& line) {
    auto fields = split(line, ';');

    if (fields.size() != 4) {
        throw std::runtime_error(
            "Invalid test case. Expected: "
            "expression;variable;interval;expected");
    }

    const auto& interval = fields[2];

    if (interval.size() < 5 || interval.front() != '[' ||
        interval.back() != ']') {
        throw std::runtime_error("Invalid interval format: " + interval);
    }

    auto bounds = split(interval.substr(1, interval.size() - 2), ':');

    if (bounds.size() != 2) {
        throw std::runtime_error("Invalid interval bounds: " + interval);
    }

    return {fields[0], fields[1], bounds[0], bounds[1],
            numathap::core::Value(std::stod(fields[3]))};
}

std::vector<IntegrateCase> loadCases(const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("Cannot open test file: " + filename);
    }

    std::vector<IntegrateCase> cases;

    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        if (line[0] == '#') continue;

        cases.push_back(parseCase(line));
    }

    return cases;
}

bool almostEqual(const numathap::core::Value& a,
                 const numathap::core::Value& b) {
    constexpr double tolerance = 1e-10;

    auto diff = a - b;

    return std::abs(diff.real().value()) < tolerance;
}

}  // namespace

TEST(IntegrateEval, FileCases) {
    using namespace numathap;

    auto cases =
        loadCases(std::string(NUMATHAP_TEST_DATA_DIR) + "/integrate_eval.txt");

    EXPECT_TRUE(!cases.empty());

    for (const auto& test : cases) {
        Context ctx;

        ctx.setInterval(test.variable, test.lower, test.upper);

        auto expression = prepare(test.expression);

        auto result = integrate(expression, test.variable, ctx);

        if (!almostEqual(result, test.expected)) {
            std::ostringstream oss;

            oss << "Expression: " << test.expression
                << "\nExpected: " << test.expected << "\nObtained: " << result;

            throw testfw::AssertionError(oss.str());
        }
    }
}

int main() { return testfw::runAll(); }