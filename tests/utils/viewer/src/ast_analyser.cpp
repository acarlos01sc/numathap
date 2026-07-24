#include "ast_analyser.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "numathap/config/Capability.hpp"
#include "numathap/config/MathEnvironment.hpp"
#include "numathap/math/MathAst.hpp"
#include "numathap/math/PreparedAst.hpp"
#include "numathap/parser/ParserAst.hpp"

namespace numathap::tools {

namespace {

// ---------------------------------------------------------------------
// Small text helpers
// ---------------------------------------------------------------------

std::string rtrim(std::string s) {
  while (!s.empty() &&
         (s.back() == '\r' || s.back() == ' ' || s.back() == '\t')) {
    s.pop_back();
  }
  return s;
}

std::string padRight(std::string s, std::size_t width) {
  if (s.size() < width) {
    s.append(width - s.size(), ' ');
  }
  return s;
}

std::vector<std::string> splitLines(const std::string& text) {
  std::vector<std::string> lines;
  std::size_t start = 0;
  while (start <= text.size()) {
    const std::size_t nl = text.find('\n', start);
    if (nl == std::string::npos) {
      lines.push_back(text.substr(start));
      break;
    }
    lines.push_back(text.substr(start, nl - start));
    start = nl + 1;
  }
  return lines;
}

std::string joinLines(const std::vector<std::string>& lines) {
  std::ostringstream out;
  for (std::size_t i = 0; i < lines.size(); ++i) {
    if (i > 0) {
      out << '\n';
    }
    out << lines[i];
  }
  return out.str();
}

// Right-trims every line, then drops leading/trailing blank lines. Used to
// make both captured print() output and YAML block-scalar snapshots
// comparable regardless of incidental surrounding whitespace.
std::string normalizeTreeText(const std::string& text) {
  std::vector<std::string> lines = splitLines(text);
  for (auto& line : lines) {
    line = rtrim(line);
  }
  while (!lines.empty() && lines.back().empty()) {
    lines.pop_back();
  }
  std::size_t start = 0;
  while (start < lines.size() && lines[start].empty()) {
    ++start;
  }
  std::ostringstream out;
  for (std::size_t i = start; i < lines.size(); ++i) {
    if (i > start) {
      out << '\n';
    }
    out << lines[i];
  }
  return out.str();
}

std::string stripQuotes(std::string v) {
  if (v.size() >= 2 &&
      ((v.front() == '"' && v.back() == '"') ||
       (v.front() == '\'' && v.back() == '\''))) {
    v = v.substr(1, v.size() - 2);
  }
  return v;
}

// ---------------------------------------------------------------------
// Minimal YAML subset parser (see ast_analyser.hpp for the schema).
// ---------------------------------------------------------------------

int indentOf(const std::string& raw) {
  int n = 0;
  while (n < static_cast<int>(raw.size()) && raw[n] == ' ') {
    ++n;
  }
  return n;
}

bool isBlankOrComment(const std::string& trimmedContent) {
  return trimmedContent.empty() || trimmedContent[0] == '#';
}

// Splits "key: value" into key/value (value may be empty, e.g. "columns:").
bool splitKeyValue(const std::string& content, std::string& key,
                    std::string& value) {
  const auto pos = content.find(':');
  if (pos == std::string::npos) {
    return false;
  }
  key = content.substr(0, pos);
  while (!key.empty() && key.back() == ' ') {
    key.pop_back();
  }
  const std::string rest = content.substr(pos + 1);
  const std::size_t start = rest.find_first_not_of(' ');
  value = (start == std::string::npos) ? std::string() : rest.substr(start);
  return true;
}

// Parses a flow list like "[Simplify]" or "[]" into its elements.
std::vector<std::string> parseFlowList(const std::string& value,
                                        std::size_t lineNo) {
  if (value.size() < 2 || value.front() != '[' || value.back() != ']') {
    throw std::runtime_error(
        "ast_analyser: expected a flow list like '[Simplify]' at line " +
        std::to_string(lineNo) + ", got: " + value);
  }
  std::vector<std::string> items;
  const std::string inner = value.substr(1, value.size() - 2);
  std::size_t start = 0;
  while (start <= inner.size()) {
    const std::size_t comma = inner.find(',', start);
    std::string item = (comma == std::string::npos)
                            ? inner.substr(start)
                            : inner.substr(start, comma - start);
    const std::size_t a = item.find_first_not_of(' ');
    const std::size_t b = item.find_last_not_of(' ');
    if (a != std::string::npos) {
      items.push_back(stripQuotes(item.substr(a, b - a + 1)));
    }
    if (comma == std::string::npos) {
      break;
    }
    start = comma + 1;
  }
  return items;
}

AstAnalyserRequest parseYamlLines(const std::vector<std::string>& rawLines) {
  AstAnalyserRequest request;
  bool haveExpression = false;

  auto lineNo = [](std::size_t idx) { return idx + 1; };

  std::size_t i = 0;
  while (i < rawLines.size()) {
    const int indent = indentOf(rawLines[i]);
    const std::string content = rtrim(rawLines[i].substr(indent));
    if (isBlankOrComment(content)) {
      ++i;
      continue;
    }
    if (indent != 0) {
      throw std::runtime_error("ast_analyser: unexpected indentation at line " +
                                std::to_string(lineNo(i)));
    }

    std::string key, value;
    if (!splitKeyValue(content, key, value)) {
      throw std::runtime_error(
          "ast_analyser: expected 'key: value' at line " +
          std::to_string(lineNo(i)));
    }

    if (key == "expression") {
      if (value.empty()) {
        throw std::runtime_error(
            "ast_analyser: 'expression' must not be empty (line " +
            std::to_string(lineNo(i)) + ")");
      }
      request.expression = stripQuotes(value);
      haveExpression = true;
      ++i;

    } else if (key == "columns") {
      if (!value.empty()) {
        throw std::runtime_error(
            "ast_analyser: 'columns' must be a block list, not an inline "
            "value (line " +
            std::to_string(lineNo(i)) + ")");
      }
      ++i;
      while (i < rawLines.size()) {
        const int itemIndent = indentOf(rawLines[i]);
        const std::string itemContent = rtrim(rawLines[i].substr(itemIndent));
        if (isBlankOrComment(itemContent)) {
          ++i;
          continue;
        }
        if (itemIndent <= indent) {
          break;
        }
        if (itemContent.rfind("- ", 0) != 0) {
          throw std::runtime_error(
              "ast_analyser: expected a '- ' list item at line " +
              std::to_string(lineNo(i)));
        }

        AstAnalyserColumnSpec spec;
        bool haveTitle = false;
        bool haveCapabilities = false;
        const int fieldIndent = itemIndent + 2;

        auto applyField = [&](const std::string& fKey, const std::string& fVal,
                               std::size_t ln) {
          if (fKey == "title") {
            spec.title = stripQuotes(fVal);
            haveTitle = true;
          } else if (fKey == "capabilities") {
            spec.capabilities = parseFlowList(fVal, ln);
            haveCapabilities = true;
          } else {
            throw std::runtime_error("ast_analyser: unknown column field '" +
                                      fKey + "' at line " + std::to_string(ln));
          }
        };

        std::string fKey, fVal;
        if (!splitKeyValue(itemContent.substr(2), fKey, fVal)) {
          throw std::runtime_error(
              "ast_analyser: expected 'title:' or 'capabilities:' at line " +
              std::to_string(lineNo(i)));
        }
        applyField(fKey, fVal, lineNo(i));
        ++i;

        while (i < rawLines.size()) {
          const int nextIndent = indentOf(rawLines[i]);
          const std::string nextContent = rtrim(rawLines[i].substr(nextIndent));
          if (isBlankOrComment(nextContent)) {
            ++i;
            continue;
          }
          if (nextIndent < fieldIndent) {
            break;
          }
          std::string k2, v2;
          if (!splitKeyValue(nextContent, k2, v2)) {
            throw std::runtime_error(
                "ast_analyser: expected 'key: value' at line " +
                std::to_string(lineNo(i)));
          }
          applyField(k2, v2, lineNo(i));
          ++i;
        }

        if (!haveTitle) {
          throw std::runtime_error(
              "ast_analyser: column entry missing 'title' (near line " +
              std::to_string(lineNo(i)) + ")");
        }
        if (!haveCapabilities) {
          spec.capabilities.clear();
        }
        request.columns.push_back(std::move(spec));
      }

    } else if (key == "expected") {
      if (!value.empty()) {
        throw std::runtime_error(
            "ast_analyser: 'expected' must be a block map, not an inline "
            "value (line " +
            std::to_string(lineNo(i)) + ")");
      }
      ++i;
      while (i < rawLines.size()) {
        const int keyIndent = indentOf(rawLines[i]);
        const std::string keyContent = rtrim(rawLines[i].substr(keyIndent));
        if (isBlankOrComment(keyContent)) {
          ++i;
          continue;
        }
        if (keyIndent <= indent) {
          break;
        }
        std::string title, marker;
        if (!splitKeyValue(keyContent, title, marker)) {
          throw std::runtime_error("ast_analyser: expected 'Title: |' at line " +
                                    std::to_string(lineNo(i)));
        }
        if (marker != "|") {
          throw std::runtime_error(
              "ast_analyser: 'expected' entries must use a literal block "
              "scalar ('Title: |'), got line " +
              std::to_string(lineNo(i)));
        }
        ++i;

        int blockIndent = -1;
        std::vector<std::string> blockLines;
        while (i < rawLines.size()) {
          const std::string& bl = rawLines[i];
          if (bl.find_first_not_of(' ') == std::string::npos) {
            if (blockIndent == -1) {
              ++i;
              continue;
            }
            blockLines.emplace_back();
            ++i;
            continue;
          }
          const int curIndent = indentOf(bl);
          if (curIndent <= keyIndent) {
            break;
          }
          if (blockIndent == -1) {
            blockIndent = curIndent;
          }
          if (curIndent < blockIndent) {
            throw std::runtime_error(
                "ast_analyser: inconsistent indentation inside block scalar "
                "for '" +
                title + "' at line " + std::to_string(lineNo(i)));
          }
          blockLines.push_back(
              rtrim(bl.substr(static_cast<std::size_t>(blockIndent))));
          ++i;
        }

        request.expectedSnapshots.emplace_back(stripQuotes(title),
                                                joinLines(blockLines));
      }

    } else {
      throw std::runtime_error("ast_analyser: unknown top-level key '" + key +
                                "' at line " + std::to_string(lineNo(i)));
    }
  }

  if (!haveExpression) {
    throw std::runtime_error(
        "ast_analyser: YAML file is missing required 'expression' key");
  }
  return request;
}

// ---------------------------------------------------------------------
// Column construction
// ---------------------------------------------------------------------

AstAnalyserColumn makeTextColumn(const std::string& title,
                                  const std::string& text) {
  AstAnalyserColumn column;
  column.title = title;
  const std::string normalized = normalizeTreeText(text);
  if (normalized.empty()) {
    column.lines.push_back("<empty>");
    return column;
  }
  column.lines = splitLines(normalized);
  return column;
}

template <typename Tree>
std::string capturePrint(const Tree& tree) {
  std::ostringstream oss;
  tree.print(oss);
  return oss.str();
}

numathap::config::Capability capabilityFromName(const std::string& name) {
  if (name == "Simplify") {
    return numathap::config::Capability::Simplify;
  }
  throw std::runtime_error("ast_analyser: unknown capability '" + name + "'");
}

}  // namespace

AstAnalyserRequest parseYamlFile(const std::string& path) {
  std::ifstream in(path);
  if (!in) {
    throw std::runtime_error("ast_analyser: cannot open YAML file: " + path);
  }
  std::vector<std::string> rawLines;
  std::string line;
  while (std::getline(in, line)) {
    if (!line.empty() && line.back() == '\r') {
      line.pop_back();
    }
    rawLines.push_back(line);
  }
  return parseYamlLines(rawLines);
}

std::vector<AstAnalyserColumn> buildColumns(const AstAnalyserRequest& request) {
  std::vector<AstAnalyserColumn> columns;

  columns.push_back(makeTextColumn("Input", request.expression));

  numathap::parser::ParserAst parserAst(request.expression);
  columns.push_back(makeTextColumn("ParserAst", capturePrint(parserAst)));

  numathap::math::MathAst mathAst(parserAst);
  columns.push_back(makeTextColumn("MathAst", capturePrint(mathAst)));

  for (const auto& spec : request.columns) {
    numathap::config::MathEnvironment env;
    for (const auto& capabilityName : spec.capabilities) {
      env.enableCapability(capabilityFromName(capabilityName));
    }
    numathap::math::PreparedAst prepared(mathAst, env);
    columns.push_back(makeTextColumn(spec.title, capturePrint(prepared)));
  }

  return columns;
}

std::string renderColumns(const std::vector<AstAnalyserColumn>& columns) {
  constexpr std::size_t kColumnGap = 3;

  std::vector<std::size_t> widths;
  widths.reserve(columns.size());
  std::size_t maxHeight = 0;
  for (const auto& col : columns) {
    std::size_t width = col.title.size();
    for (const auto& line : col.lines) {
      width = std::max(width, line.size());
    }
    widths.push_back(width);
    maxHeight = std::max(maxHeight, col.lines.size());
  }

  std::size_t totalWidth = 0;
  for (const std::size_t w : widths) {
    totalWidth += w;
  }
  if (!widths.empty()) {
    totalWidth += (widths.size() - 1) * kColumnGap;
  }

  std::ostringstream out;
  for (std::size_t i = 0; i < columns.size(); ++i) {
    if (i > 0) {
      out << std::string(kColumnGap, ' ');
    }
    out << padRight(columns[i].title, widths[i]);
  }
  out << '\n' << std::string(totalWidth, '=') << '\n';

  for (std::size_t row = 0; row < maxHeight; ++row) {
    for (std::size_t i = 0; i < columns.size(); ++i) {
      if (i > 0) {
        out << std::string(kColumnGap, ' ');
      }
      const std::string cell =
          row < columns[i].lines.size() ? columns[i].lines[row] : std::string();
      out << padRight(cell, widths[i]);
    }
    out << '\n';
  }
  return out.str();
}

std::string runDisplayMode(const std::string& yamlPath) {
  const AstAnalyserRequest request = parseYamlFile(yamlPath);
  return renderColumns(buildColumns(request));
}

bool runTestMode(const std::string& yamlPath, std::string& diffReport) {
  const AstAnalyserRequest request = parseYamlFile(yamlPath);
  const std::vector<AstAnalyserColumn> columns = buildColumns(request);

  std::ostringstream report;
  bool allMatch = true;

  for (const auto& column : columns) {
    if (column.title == "Input") {
      continue;  // nothing to snapshot for the echoed expression
    }

    const auto expectedIt = std::find_if(
        request.expectedSnapshots.begin(), request.expectedSnapshots.end(),
        [&](const auto& kv) { return kv.first == column.title; });

    if (expectedIt == request.expectedSnapshots.end()) {
      allMatch = false;
      report << "Column '" << column.title
             << "': no 'expected' snapshot provided in YAML.\n";
      continue;
    }

    const std::string actual = joinLines(column.lines);
    const std::string expected = normalizeTreeText(expectedIt->second);
    if (actual != expected) {
      allMatch = false;
      report << "Column '" << column.title << "': mismatch\n"
             << "--- expected ---\n"
             << expected << '\n'
             << "--- actual ---\n"
             << actual << '\n';
    }
  }

  diffReport = report.str();
  return allMatch;
}

}  // namespace numathap::tools

int main(int argc, char** argv) {
  std::vector<std::string> args(argv + 1, argv + argc);
  bool testMode = false;
  std::string yamlPath;

  for (const auto& arg : args) {
    if (arg == "--test") {
      testMode = true;
    } else if (yamlPath.empty()) {
      yamlPath = arg;
    } else {
      std::cerr << "ast_analyser: unexpected argument '" << arg << "'\n";
      return 2;
    }
  }

  if (yamlPath.empty()) {
    std::cerr << "usage: ast_analyser <file.yaml> [--test]\n";
    return 2;
  }

  try {
    if (testMode) {
      std::string diffReport;
      const bool ok = numathap::tools::runTestMode(yamlPath, diffReport);
      if (!ok) {
        std::cerr << diffReport;
      }
      return ok ? 0 : 1;
    }
    std::cout << numathap::tools::runDisplayMode(yamlPath);
    return 0;
  } catch (const std::exception& e) {
    std::cerr << "ast_analyser: " << e.what() << '\n';
    return 2;
  }
}
