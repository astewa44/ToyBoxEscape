#include <catch.hpp>
#include "Ranges.hpp"

#include <vector>

TEST_CASE("deferred_iterator works", "[Ranges]")
{
    SECTION("dereferencing a deferred_iterator returns the base iterator")
    {
        std::vector<int> vec;
        auto iter = make_deferred_iterator(begin(vec));
        REQUIRE(*iter == begin(vec));
    }

    SECTION("defer() returns proper range")
    {
        std::vector<int> vec;
        auto rng = defer(vec);
        REQUIRE(begin(rng) == make_deferred_iterator(begin(vec)));
        REQUIRE(end(rng) == make_deferred_iterator(end(vec)));
        REQUIRE(*begin(rng) == begin(vec));
        REQUIRE(*end(rng) == end(vec));
    }
}
