#ifndef GREPENGINE_H
#define GREPENGINE_H

#include <string>
#include <vector>
#include <regex>
#include "GrepOptions.h"

class GrepEngine {
public:
    GrepEngine(const GrepOptions& options);
    void searchFiles(const std::vector<std::string>& files);

private:
    GrepOptions options;

    void searchInFile(const std::string& filename);
    void printMatch(const std::string& filename, const std::string& line, int lineNumber, const std::smatch& match);
    bool matchesPattern(const std::string& line, std::smatch& match);
};

#endif
