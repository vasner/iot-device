/**
 * Main module
 */

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "log.h"

#define LOG_LEVEL (LOG_LEVEL_WARN)

int main(int argc, char* argv[]) {
    log_set_level(LOG_LEVEL);
    int result = Catch::Session().run(argc, argv);
    return result;
}
