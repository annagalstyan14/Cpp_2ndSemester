#ifndef GREP_OPTIONS_H
#define GREP_OPTIONS_H

#include <string>
#include <vector>

class GrepOptions {
public:
    std::string pattern;
    std::vector<std::string> files;
    std::vector<std::string> patterns;
    bool ignoreCase;
    bool invertMatch;
    bool showCount;
    bool noFilename;
    bool onlyFilenames;
    bool lineNumbers;
    bool extendRegex;
    bool wordMatchOnly;
    bool onlyMatching;

    int afterContext;
    int beforeContext;
    int context;

    GrepOptions();

    void parseArgs(int argc, char* argv[]);

private:
    void loadPatternFile(const std::string& filename);
};

#endif
