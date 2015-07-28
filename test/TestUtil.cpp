#include <catch.hpp>
#include "Util.hpp"

TEST_CASE("Clamp() returns correct values", "[Util]")
{
    SECTION("clamping a number from below the range returns the start of the range")
    {
        CHECK(Clamp(-15, -10, 10) == -10);
    }
    SECTION("clamping a number equal to the start of the range returns the start of the range")
    {
        CHECK(Clamp(-10, -10, 10) == -10);
    }
    SECTION("clamping a number from within the range returns that number")
    {
        CHECK(Clamp(0, -10, 10) == 0);
    }
    SECTION("clamping a number equal to the end of the range returns the end of the range")
    {
        CHECK(Clamp(10, -10, 10) == 10);
    }
    SECTION("clamping a number from above the range returns the end of the range")
    {
        CHECK(Clamp(15, -10, 10) == 10);
    }
}

TEST_CASE("Wrap() returns correct values", "[Util]")
{
    SECTION("wrapping a number from below the range returns the start of the range")
    {
        CHECK(Wrap(-15, -10, 10) == 5);
    }
    SECTION("wrapping a number equal to the start of the range returns the start of the range")
    {
        CHECK(Wrap(-10, -10, 10) == -10);
    }
    SECTION("wrapping a number from within the range returns that number")
    {
        CHECK(Wrap(0, -10, 10) == 0);
    }
    SECTION("wrapping a number equal to the end of the range returns the start of the range")
    {
        CHECK(Wrap(10, -10, 10) == -10);
    }
    SECTION("wrapping a number from above the range returns a number within the range")
    {
        CHECK(Wrap(15, -10, 10) == -5);
    }
    SECTION("wrapping a number can result in the start of the range")
    {
        CHECK(Wrap(30, -10, 10) == -10);
    }
    SECTION("if wrapping a number would result in the end of the range, the start of the range is returned")
    {
        CHECK(Wrap(30, -10, 10) == -10);
    }
}

TEST_CASE("Sign() returns correct values", "[Util]")
{
    SECTION("Sign() of a negative integer is -1")
    {
        CHECK(Sign(-15) == -1);
    }
    SECTION("Sign() of a zero integer is 0")
    {
        CHECK(Sign(0) == 0);
    }
    SECTION("Sign() of a positive integer is 1")
    {
        CHECK(Sign(15) == 1);
    }
    SECTION("Sign() of a zero unsigned integer is 0u")
    {
        CHECK(Sign(0u) == 0u);
    }
    SECTION("Sign() of a positive unsigned integer is 1u")
    {
        CHECK(Sign(15u) == 1u);
    }
    SECTION("Sign() of a negative float is -1.0")
    {
        CHECK(Sign(-15.0) == -1.0);
    }
    SECTION("Sign() of a negative zero float is 0.0")
    {
        CHECK(Sign(-0.0) == 0.0);
    }
    SECTION("Sign() of a positive zero float is 0.0")
    {
        CHECK(Sign(0.0) == 0.0);
    }
    SECTION("Sign() of a positive float is 1.0")
    {
        CHECK(Sign(15.0) == 1.0);
    }
}
