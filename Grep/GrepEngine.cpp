#include "GrepEngine.h"
#include <fstream>
#include <iostream>

GrepEngine::GrepEngine(const GrepOptions& options) : options(options) {}

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
    while (std::getline(infile, line)) {
        lineNumber++;
        std::smatch match;

        if (matchesPattern(line, match)) {
            printMatch(filename, line, lineNumber, match);
        }
    }
    infile.close();
}

bool GrepEngine::matchesPattern(const std::string& line, std::smatch& match) {
    std::regex_constants::syntax_option_type flags = std::regex::ECMAScript;
    if (options.ignoreCase) {
        flags |= std::regex::icase;
    }

    try {
        std::regex re(options.pattern, flags);
        return std::regex_search(line, match, re);
    }
    catch (const std::regex_error& e) {
        std::cerr << "Regex error: " << e.what() << std::endl;
        return false;
    }
}

void GrepEngine::printMatch(const std::string& filename, const std::string& line, int lineNumber, const std::smatch& match) {
    if (!options.noFilename) {
        std::cout << filename << ":";
    }

    if (options.lineNumbers) {
        std::cout << lineNumber << ":";
    }

    if (options.onlyMatching) {
        for (const auto& m : match) {
            std::cout << m << std::endl;
        }
    }
    else {
        std::cout << line << std::endl;
    }
}