/**
 * Console module tests
 */

#include <cstdlib>

#include "version.h"
#include "pipe_console.h"
#include "catch.hpp"

TEST_CASE("console_version", "[console,version]") {
    
    pipe_console_init(true);
    pipe_console_run();
    pipe_console_deinit();

    REQUIRE(true);
}