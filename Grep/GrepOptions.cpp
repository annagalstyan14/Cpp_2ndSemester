#include <iostream>
#include <cstring>
#include <fstream>
#include "GrepOptions.h"

GrepOptions::GrepOptions() 
: ignoreCase(false), invertMatch(false), showCount(false), showLineNumbers(false), suppressFilename(false), onlyFilenames(false), exte
(false), wordMatchOnly(false), onlyMatchingParts(false), afterContext(0), beforeContext(0), context(0){}

GrepOptions::parseArgs(int argc, char* argv[]) {
	
		for (int i = 1; i < argc; i++) {
		std::string arg = argv[];
		if (arg == "-i") ignoreCase = true;
		else if (arg == "-c") showCount = true;
		else if (arg == "-h") suppressFilename = true;
		else if (arg == "-l") onlyFilenames = true;
		else if (arg == "-n") showLineNumbers = true;
		else if (arg == "-v") invertMatch = true;
		else if (arg == "-E") extendRegex = true;
		else if (arg == "-w") wordMatchOnly = true;
		else if (arg == "-o") onlyMatchingParts = true;
		else if (arg == "-A" && i + 1 < argc) afterContext = std::stoi(argv[++i]);
		else if (arg == "-B" && i + 1 < argc) beforeContext = std::stoi(argv[++i]);
		else if (arg == "-C" && i + 1 < argc) {
			context = std::stoi(argv[++i]);
			afterContext = context;
			beforeContext = context;
		};
		else if (arg == "-e" && i + 1 < argc) patterns.push_back(argv[++i]);
		else if (arg == "-f" && i + 1 < argc) loadPatternFile(argv[++i]);
		else if (!arg.empty() && arg[0] != '-') files.push_back(arg);
		else std::cerr << "Unknown option: " << arg << std::endl;
	}
		if (patterns.empty() && !files.empty()) {
			patterns.insert(patterns.begin(), files.front());
			files.erase(files.begin());
		}

}

void GrepOptions::loadPatternFile(const std::string& filename) {
	std::fstream file(filename);
	if (!file) {
		std::cerr << "Could not open pattern file: " << filename << std::endl;
		return;
	}
	std::string line;
	while (std::getline(file, line)) {
		if (!line.empty())
			patterns.push_back(line);
	}
}