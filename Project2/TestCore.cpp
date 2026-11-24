#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_session.hpp>
#include <memory>
#include <stdexcept>
#include "Vector.h"
#include "GridField.h"
#include "FieldSystem.h"
#include "GeometryStats.h"
#include "GeometrySystem.h"
#include "Unit.h"
#include "Personnel.h"
#include "Vehicle.h"
#include "World.h"

int main(int argc, char* argv[])
{
    return Catch::Session().run(argc, argv);
}

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

TEST_CASE("FieldSystem initializes an empty rectangular influence grid", "[FieldSystem]")
{
    FieldSystem field(3, 2);
    const World world;

    field.calculateInfluence(world);
    const auto& influence = field.getInfluence();

    REQUIRE(influence.width() == 3);
    REQUIRE(influence.height() == 2);
    for (std::size_t row = 0; row < influence.height(); ++row)
    {
        for (std::size_t column = 0; column < influence.width(); ++column)
        {
            CHECK(influence.at(column, row) == 0.0f);
        }
    }
}

TEST_CASE("FieldSystem predicts signed movement without modifying the influence grid", "[FieldSystem]")
{
    const FieldSystem field(3, 1);
    const Math::Vector origin(0, 0);
    const Math::Vector destination(2, 0);
    const Unit firstTeam(origin, 0, 2);
    const Unit secondTeam(origin, 1, 2);

    const auto stationary = field.predictMove(firstTeam, origin);
    const auto firstPrediction = field.predictMove(firstTeam, destination);
    const auto secondPrediction = field.predictMove(secondTeam, destination);

    CHECK(firstPrediction.at(0, 0) < 0.0f);
    CHECK(firstPrediction.at(1, 0) == 0.0f);
    CHECK(firstPrediction.at(2, 0) == Catch::Approx(-firstPrediction.at(0, 0)));
    for (std::size_t column = 0; column < field.getInfluence().width(); ++column)
    {
        CHECK(secondPrediction.at(column, 0) == Catch::Approx(-firstPrediction.at(column, 0)));
        CHECK(stationary.at(column, 0) == 0.0f);
        CHECK(field.getInfluence().at(column, 0) == 0.0f);
    }
}

TEST_CASE("World owns added units and exposes them to FieldSystem", "[World][FieldSystem]")
{
    World world(3, 2);
    world.addUnit(std::make_unique<Unit>(Math::Vector(1, 0), 0, 2));

    REQUIRE(world.getUnits().size() == 1);
    CHECK_THROWS_AS(world.addUnit(nullptr), std::invalid_argument);
    CHECK(world.getUnits().size() == 1);

    FieldSystem field(3, 2);
    field.calculateInfluence(world);
    CHECK(field.getInfluence().at(1, 0) > 0.0f);
}

TEST_CASE("Personnel and vehicles use polymorphic territorial control strength", "[Unit][FieldSystem]")
{
    std::unique_ptr<Unit> personnel = std::make_unique<Personnel>(Math::Vector(0, 0), 0, 6.0);
    std::unique_ptr<Unit> vehicle = std::make_unique<Vehicle>(Math::Vector(0, 0), 0, 6.0, 1.0);

    CHECK(personnel->getInitialStrength() == 6.0);
    CHECK(vehicle->getInitialStrength() == 6.0);
    CHECK(personnel->getControlStrength() == 6.0);
    CHECK(vehicle->getControlStrength() == 6.0);

    personnel->receiveDamage(3.0);
    vehicle->receiveDamage(3.0);

    CHECK(personnel->getControlStrength() == Catch::Approx(1.5));
    CHECK(vehicle->getControlStrength() == 3.0);

    personnel->receiveDamage(3.0);
    vehicle->receiveDamage(3.0);

    CHECK(personnel->getControlStrength() == 0.0);
    CHECK(vehicle->getControlStrength() == 0.0);
}

TEST_CASE("GeometryStats defaults all counters to zero", "[GeometryStats]")
{
    const GeometryStats stats;

    CHECK(stats.area == 0);
    CHECK(stats.perimeter == 0);
    CHECK(stats.components == 0);
    CHECK(stats.holes == 0);
}

TEST_CASE("GeometrySystem reports a ring and a separate component for one team", "[GeometryStats]")
{
    GridField<float> influence(5, 3, 0.0f);
    for (std::size_t row = 0; row < 3; ++row)
    {
        for (std::size_t column = 0; column < 3; ++column)
        {
            influence.at(column, row) = 1.0f;
        }
    }
    influence.at(1, 1) = -1.0f;
    influence.at(4, 1) = 1.0f;

    const GeometrySystem geometry;
    const auto firstTeam = geometry.analyzeGeometry(0, influence);
    const auto secondTeam = geometry.analyzeGeometry(1, influence);

    CHECK(firstTeam.area == 9);
    CHECK(firstTeam.perimeter == 20);
    CHECK(firstTeam.components == 2);
    CHECK(firstTeam.holes == 1);
    CHECK(secondTeam.area == 1);
    CHECK(secondTeam.perimeter == 4);
    CHECK(secondTeam.components == 1);
    CHECK(secondTeam.holes == 0);
}

