#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <bytepack/bytepack.hpp>

TEST_CASE("Your first test", "[bytepack]")
{
    REQUIRE( 1 == 1 );
}
