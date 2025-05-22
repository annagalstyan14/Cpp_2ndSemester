#include "GrepOptions.h"
#include "GrepEngine.h"

int main(int argc, char* argv[]) {
    GrepOptions options;
    options.parse(argc, argv);

    GrepEngine engine(options);
    engine.searchFiles(options.files);

    return 0;
}