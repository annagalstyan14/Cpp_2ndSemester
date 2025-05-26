#include "GrepOptions.h"
#include "GrepEngine.h"

int main(int argc, char* argv[]) {
    GrepOptions options;
    options.parseArgs(argc, argv);

    if (options.patterns.empty() && !options.files.empty()) {
        options.patterns.push_back(options.files.front());
        options.files.erase(options.files.begin());
    }

    GrepEngine engine(options);
    engine.searchFiles(options.files);

    return 0;
}
