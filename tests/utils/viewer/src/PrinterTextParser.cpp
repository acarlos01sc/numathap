#include "PrinterTextParser.hpp"

#include <sstream>
#include <stdexcept>

namespace numathap::viewer {

namespace {

int countLeadingSpaces(const std::string& line) {
    std::size_t i = 0;
    while (i < line.size() && line[i] == ' ') {
        ++i;
    }
    return static_cast<int>(i);
}

bool isBlank(const std::string& line) {
    return line.find_first_not_of(" \t\r\n") == std::string::npos;
}

bool hasConnectorGlyphs(const std::string& text) {
    return text.find("\u251C") != std::string::npos ||  // "├"
           text.find("\u2514") != std::string::npos ||  // "└"
           text.find("\u2502") != std::string::npos;    // "│"
}

}  // namespace

std::vector<PrinterTextParser::Line> PrinterTextParser::toLines(
    const std::string& text) const {
    std::vector<std::string> rawLines;
    std::istringstream stream(text);
    std::string raw;

    while (std::getline(stream, raw)) {
        if (!raw.empty() && raw.back() == '\r') {
            raw.pop_back();  // tolerate CRLF input
        }
        if (isBlank(raw)) {
            continue;
        }
        rawLines.push_back(raw);
    }

    if (rawLines.empty()) {
        throw std::invalid_argument(
            "PrinterTextParser: cannot parse empty or blank printer output");
    }

    std::vector<Line> lines = hasConnectorGlyphs(text)
                                  ? parseConnectorStyle(rawLines)
                                  : parseIndentStyle(rawLines);

    if (lines.front().depth != 0) {
        throw std::invalid_argument(
            "PrinterTextParser: first line must be at depth 0 (the tree root)");
    }

    return lines;
}

std::vector<PrinterTextParser::Line> PrinterTextParser::parseIndentStyle(
    const std::vector<std::string>& rawLines) const {
    std::vector<int> rawSpaces;
    std::vector<std::string> labels;
    rawSpaces.reserve(rawLines.size());
    labels.reserve(rawLines.size());

    for (const auto& raw : rawLines) {
        const int leadingSpaces = countLeadingSpaces(raw);
        rawSpaces.push_back(leadingSpaces);
        labels.push_back(raw.substr(static_cast<std::size_t>(leadingSpaces)));
    }

    const int indentUnit = detectIndentUnit(rawSpaces);

    std::vector<Line> lines;
    lines.reserve(rawSpaces.size());
    for (std::size_t i = 0; i < rawSpaces.size(); ++i) {
        if (rawSpaces[i] % indentUnit != 0) {
            throw std::invalid_argument(
                "PrinterTextParser: leading whitespace at line " +
                std::to_string(i) +
                " does not align to the detected "
                "indentation unit (" +
                std::to_string(indentUnit) + " spaces)");
        }
        lines.push_back(Line{rawSpaces[i] / indentUnit, labels[i]});
    }

    return lines;
}

std::vector<PrinterTextParser::Line> PrinterTextParser::parseConnectorStyle(
    const std::vector<std::string>& rawLines) const {
    static const std::string kLast = "\u2514\u2500\u2500 ";    // "└── "
    static const std::string kBranch = "\u251C\u2500\u2500 ";  // "├── "
    static const std::string kVert = "\u2502   ";              // "│   "
    static const std::string kBlank = "    ";

    std::vector<Line> lines;
    lines.reserve(rawLines.size());

    for (std::size_t i = 0; i < rawLines.size(); ++i) {
        std::string rest = rawLines[i];
        int chunks = 0;

        while (true) {
            if (rest.compare(0, kLast.size(), kLast) == 0) {
                rest.erase(0, kLast.size());
            } else if (rest.compare(0, kBranch.size(), kBranch) == 0) {
                rest.erase(0, kBranch.size());
            } else if (rest.compare(0, kVert.size(), kVert) == 0) {
                rest.erase(0, kVert.size());
            } else if (rest.compare(0, kBlank.size(), kBlank) == 0) {
                rest.erase(0, kBlank.size());
            } else {
                break;
            }
            ++chunks;
        }

        if (rest.rfind("\u251C", 0) == 0 || rest.rfind("\u2514", 0) == 0 ||
            rest.rfind("\u2502", 0) == 0) {
            throw std::invalid_argument(
                "PrinterTextParser: malformed connector prefix at line " +
                std::to_string(i));
        }

        // The root line never prints its own connector (see class doc),
        // so every real depth is one less than the raw chunk count.
        const int depth = (chunks == 0) ? 0 : chunks - 1;
        lines.push_back(Line{depth, rest});
    }

    return lines;
}

int PrinterTextParser::detectIndentUnit(
    const std::vector<int>& leadingSpaces) const {
    int minPositiveDelta = 0;

    for (std::size_t i = 1; i < leadingSpaces.size(); ++i) {
        const int delta = leadingSpaces[i] - leadingSpaces[i - 1];
        if (delta > 0 && (minPositiveDelta == 0 || delta < minPositiveDelta)) {
            minPositiveDelta = delta;
        }
    }

    return minPositiveDelta > 0 ? minPositiveDelta : 1;
}

RenderNode PrinterTextParser::parseNode(const std::vector<Line>& lines,
                                        std::size_t& index, int depth) const {
    if (lines[index].depth != depth) {
        throw std::invalid_argument(
            "PrinterTextParser: malformed indentation - expected depth " +
            std::to_string(depth) + " at line " + std::to_string(index));
    }

    RenderNode node;
    node.label = lines[index].label;
    ++index;

    while (index < lines.size() && lines[index].depth > depth) {
        if (lines[index].depth != depth + 1) {
            throw std::invalid_argument(
                "PrinterTextParser: malformed indentation - depth jumped "
                "from " +
                std::to_string(depth) + " to " +
                std::to_string(lines[index].depth) + " at line " +
                std::to_string(index) + " (expected " +
                std::to_string(depth + 1) + ")");
        }
        node.children.push_back(parseNode(lines, index, depth + 1));
    }

    return node;
}

RenderNode PrinterTextParser::parse(const std::string& text) const {
    const std::vector<Line> lines = toLines(text);

    std::size_t index = 0;
    RenderNode root = parseNode(lines, index, 0);

    if (index != lines.size()) {
        throw std::invalid_argument(
            "PrinterTextParser: text contains more than one root-level node "
            "(unexpected content after the first tree)");
    }

    return root;
}

}  // namespace numathap::viewer