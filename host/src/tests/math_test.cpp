/**
 * Math library tests
 */

#include "catch.hpp"
#include "math_functions.h"

TEST_CASE("sc_mul_round_s32", "[math][scalar]") {
    int32_t result = sc_mul_round_s32(MATH_CONST_S32(0.5f, 20), MATH_CONST_S32(0.7f, 20), 20);
    REQUIRE(result == Approx(MATH_CONST_S32(0.35f, 20)).epsilon(0.01));
}
