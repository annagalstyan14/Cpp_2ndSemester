#include "GrepEngine.h"
#include <fstream>
#include <iostream>

GrepEngine::GrepEngine(const GrepOptions& options) : options(options) {
    buffer.resize(options.beforeContext + options.afterContext + 1);
}

void GrepEngine::searchFiles(const std::vector<std::string>& files) {
    for (const auto& file : files) {
        searchInFile(file);
    }
}

void GrepEngine::searchInFile(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return;
    }

    std::string line;
    int lineNumber = 0;
    int matchCount = 0;
    bool hasMatch = false;

    while (std::getline(infile, line)) {
        lineNumber++;
        buffer[lineNumber % buffer.size()] = line;

        std::smatch match;
        bool matched = false;
        for (const auto& pattern : options.patterns) {
            if (matchesPattern(line, match)) {
                matched = true;
                break;
            }
        }

        if (options.invertMatch) matched = !matched;

        if (matched) {
            hasMatch = true;
            matchCount++;
            printContext(lineNumber, filename);
            printMatch(filename, line, lineNumber, match);
        } else if (!options.onlyFilenames) {
            buffer[lineNumber % buffer.size()] = ""; // Clear buffer for non-matches if not -l
        }
    }

    if (options.showCount) {
        if (!options.noFilename && !options.onlyFilenames) std::cout << filename << ":";
        std::cout << matchCount << std::endl;
    } else if (options.onlyFilenames && hasMatch) {
        std::cout << filename << std::endl;
    }

    infile.close();
}

bool GrepEngine::matchesPattern(const std::string& line, std::smatch& match) {
    std::regex_constants::syntax_option_type flags = std::regex::ECMAScript;
    if (options.ignoreCase) flags |= std::regex::icase;
    if (options.extendRegex) flags |= std::regex::extended; 

    std::string pattern = options.pattern;
    if (options.wordMatchOnly) pattern = "\\b" + pattern + "\\b";

    try {
        std::regex re(pattern, flags);
        return std::regex_search(line, match, re);
    } catch (const std::regex_error& e) {
        std::cerr << "Regex error: " << e.what() << std::endl;
        return false;
    }
}

void GrepEngine::printMatch(const std::string& filename, const std::string& line, int lineNumber, const std::smatch& match) {
    if (!options.noFilename && !options.onlyFilenames) std::cout << filename << ":";
    if (options.lineNumbers) std::cout << lineNumber << ":";
    if (options.onlyMatching) {
        std::cout << match[0] << std::endl;
    } else {
        std::cout << line << std::endl;
    }
}

void GrepEngine::printContext(int lineNumber, const std::string& filename) {
    int start = lineNumber - options.beforeContext - 1;
    int end = lineNumber + options.afterContext - 1;
    for (int i = start; i <= end; ++i) {
        if (i < 0 || i >= lineNumber || buffer[i % buffer.size()].empty()) continue;
        if (!options.noFilename) std::cout << filename << ":";
        if (options.lineNumbers) std::cout << (i + 1) << ":";
        std::cout << buffer[i % buffer.size()] << std::endl;
    }
}
