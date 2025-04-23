#include "app.h"
#include "config.hpp"

int main(int argc, char** argv) {
    AppConfig config = parseCommandLine(argc, argv);
    MandelbrotApp app(config);
    return app.run();
}
