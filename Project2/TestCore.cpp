#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "Vector.h"
#include "GridField.h"
// TEST 1: Vector
TEST_CASE("Vector arithmetic and distance", "[Math]")
{
    Math::Vector a(3, 4);
    Math::Vector b(1, 2);
    Math::Vector difference = a - b;
    REQUIRE(difference.length() == Catch::Approx(2.828427f).epsilon(0.0001));
    Math::Vector zero = a - a;
    REQUIRE(zero.length() == Catch::Approx(0.0f));
}
// TEST 2: GridField
TEST_CASE("GridField stores independent cells", "[GridField]")
{
    GridField<float> field(3, 3, 0.0f);
    field.at(0, 0) = 10.0f;
    field.at(2, 2) = -5.0f;
    REQUIRE(field.at(0, 0) == 10.0f);
    REQUIRE(field.at(2, 2) == -5.0f);
    REQUIRE(field.at(1, 1) == 0.0f);
}
/// FiledSystem,GeomtryStats