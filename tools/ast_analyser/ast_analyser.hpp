/**
 * @file ast_analyser.hpp
 * @brief Development/CI support tool: renders ParserAst, MathAst, and a
 * configurable set of PreparedAst columns side by side for a single
 * expression, and can diff them against YAML snapshots in `--test` mode.
 *
 * Input is a YAML file with the following shape:
 *
 * @code
 * expression: "1e12*1"
 * columns:
 *   - title: "PreparedAst"
 *     capabilities: []
 *   - title: "PreparedAst(Simplify)"
 *     capabilities: [Simplify]
 * # 'expected' is only required in --test mode.
 * expected:
 *   ParserAst: |
 *     Binary(*)
 *       Number(1e12)
 *       Number(1)
 *   MathAst: |
 *     Binary(*)
 *     ├── Number(1e12)
 *     └── Number(1)
 *   PreparedAst: |
 *     Binary(*)
 *     ├── Number(1e12)
 *     └── Number(1)
 *   PreparedAst(Simplify): |
 *     Number(1e12)
 * @endcode
 *
 * `expression` is required. `columns` is optional and lists, in display
 * order, every PreparedAst column beyond the always-present ParserAst and
 * MathAst columns; each entry's `title` is shown as-is in the header, and
 * `capabilities` (a flow list, possibly empty) names which
 * `numathap::config::Capability` values are enabled on that column's
 * MathEnvironment before building the PreparedAst.
 *
 * This tool only ever touches numathap through each tree's public
 * `print(std::ostream&)` — it never inspects internal node types, and it
 * intentionally does NOT reformat the printed text: each column keeps the
 * exact visual style (indentation or `├──`/`└──` connectors) that its own
 * printer produces.
 *
 * This is a hand-rolled parser for the specific YAML subset above, not a
 * general-purpose YAML implementation — the project has no YAML library
 * dependency, and the schema is small and stable enough not to need one.
 */

#pragma once

#include <string>
#include <utility>
#include <vector>

namespace numathap::tools {

/**
 * @brief One optional column requested in the YAML `columns:` list.
 */
struct AstAnalyserColumnSpec {
  /** Column header text, shown exactly as given. */
  std::string title;

  /**
   * Names of the `numathap::config::Capability` values to enable (e.g.
   * "Simplify") before building this column's PreparedAst. Empty means a
   * plain PreparedAst with no capability enabled.
   */
  std::vector<std::string> capabilities;
};

/**
 * @brief A parsed `ast_analyser` YAML input file.
 */
struct AstAnalyserRequest {
  /** Expression to parse and prepare. */
  std::string expression;

  /** Optional columns beyond the always-present ParserAst/MathAst. */
  std::vector<AstAnalyserColumnSpec> columns;

  /**
   * Expected printed text per column title, as given under `expected:`.
   * Only populated (and only required) when running in `--test` mode.
   * Column titles here must match "ParserAst", "MathAst", or one of the
   * titles in `columns`.
   */
  std::vector<std::pair<std::string, std::string>> expectedSnapshots;
};

/**
 * @brief One rendered column of the side-by-side layout.
 *
 * `lines` is already the exact visual form of the column, one entry per
 * printed row, in the tree's own native printer style. An empty tree
 * (blank `print()` output) is represented as a single-element vector
 * containing exactly `"<empty>"`, so callers never need to special-case
 * emptiness themselves.
 */
struct AstAnalyserColumn {
  std::string title;
  std::vector<std::string> lines;
};

/**
 * @brief Parses `path` as an ast_analyser YAML input file.
 * @throws std::runtime_error if the file cannot be opened or is malformed.
 */
[[nodiscard]] AstAnalyserRequest parseYamlFile(const std::string& path);

/**
 * @brief Builds the side-by-side columns for `request`: Input, ParserAst,
 * MathAst, then one column per entry in `request.columns`, in order.
 * @throws std::runtime_error if a column names an unknown capability, or
 * propagated from numathap if `request.expression` cannot be parsed.
 */
[[nodiscard]] std::vector<AstAnalyserColumn> buildColumns(
    const AstAnalyserRequest& request);

/**
 * @brief Renders `columns` as a single text block: a title row, a `=`
 * separator row spanning the full width, then one row per line — with
 * automatic per-column width and shorter columns padded with blank cells
 * to support trees of different heights.
 */
[[nodiscard]] std::string renderColumns(
    const std::vector<AstAnalyserColumn>& columns);

/**
 * @brief Normal (display) mode: parses `yamlPath`, builds the columns, and
 * returns them rendered as a single text block ready to print.
 */
[[nodiscard]] std::string runDisplayMode(const std::string& yamlPath);

/**
 * @brief `--test` mode: parses `yamlPath` (which must include `expected`
 * snapshots for every built column except Input), builds the columns, and
 * diffs each one's printed text against its expected snapshot.
 *
 * @param yamlPath Path to the YAML input file.
 * @param diffReport Receives a human-readable description of any
 * mismatches or missing snapshots found; empty when everything matches.
 * @return true if every column matches its snapshot (caller should exit
 * 0), false otherwise (caller should exit 1).
 * @throws std::runtime_error on malformed YAML or an unbuildable
 * expression — this is a distinct failure mode from a snapshot mismatch
 * and should be reported as an error, not as a 0/1 test result.
 */
[[nodiscard]] bool runTestMode(const std::string& yamlPath,
                                std::string& diffReport);

}  // namespace numathap::tools
