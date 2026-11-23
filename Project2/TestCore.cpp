#ifdef BUILD_SIMULATION

#include <iostream>
#include <memory>
#include <vector>
#include "Commander.h"
#include "CombatSystem.h"
#include "FieldSystem.h"
#include "GeometrySystem.h"
#include "Simulation.h"
#include "Unit.h"
#include "Personnel.h"
#include "World.h"

int main()
{
    World world(60, 60);
    // This scenario contains only personnel units.
    world.addUnit(std::make_unique<Personnel>(
        Math::Vector(3, 8), 0, 6.0, std::vector<double>{ 1.6, 0.0, 0.2 }, 5.0, 1.0, 0.5));
    world.addUnit(std::make_unique<Personnel>(
        Math::Vector(7, 22), 0, 6.0, std::vector<double>{ 1.6, 0.0, 0.2 }, 5.0, 1.0, 0.5));
    world.addUnit(std::make_unique<Personnel>(
        Math::Vector(18, 10), 1, 6.0, std::vector<double>{ 0.4, 0.0, 1.5 }, 5.0, 1.5, 1.2));
    world.addUnit(std::make_unique<Personnel>(
        Math::Vector(24, 20), 1, 6.0, std::vector<double>{ 0.4, 0.0, 1.5 }, 5.0, 1.5, 1.2));

    FieldSystem field(60, 60);
    GeometrySystem geometry;
    Commander commander;
    CombatSystem combat;
    Simulation simulation(world, field, geometry, commander, combat);

    for (int step = 0; step < 30; ++step)
    {
        // Demonstration: first prioritize area, then risk, then balance both.
        const double coefA = step < 3 ? 12.0 : step < 6 ? 0.05 : 1.0;
        const double coefR = step < 3 ? 0.05 : step < 6 ? 12.0 : 1.0;
        for (const auto& unit : world.getUnits())
            unit->setCoefficients(coefA, coefR);

        simulation.tick();
        const auto firstTeam = geometry.analyzeGeometry(0, field.getInfluence());
        const auto secondTeam = geometry.analyzeGeometry(1, field.getInfluence());
        std::cout << "Step " << step + 1 << ": team 0 area=" << firstTeam.area
            << ", team 1 area=" << secondTeam.area
            << ", coefA=" << coefA << ", coefR=" << coefR << '\n';
        for (const auto& unit : world.getUnits())
        {
            const auto score = commander.scorePosition(world, *unit, field, geometry, unit->getPos());
            std::cout << "  team " << unit->getTeam() << " at ("
                << unit->getPos().getX() << ", " << unit->getPos().getY()
                << "), strength=" << unit->getStrength() << '\n';
            const auto& profile = unit->getCombatProfile();
            std::cout << "    combatProfile: close=" << profile[0]
                << ", mid=" << profile[1] << ", long=" << profile[2] << '\n';
            std::cout << "    score: area=" << score.area << ", risk=" << score.risk
                << ", total=" << score.total << '\n';
        }
    }

    std::cout << "Final map (A=team 0, B=team 1, +=team 0 influence, -=team 1 influence):\n";
    for (std::size_t y = 0; y < field.getInfluence().height(); ++y)
    {
        for (std::size_t x = 0; x < field.getInfluence().width(); ++x)
        {
            const float value = field.getInfluence().at(x, y);
            char cell = value > 0.055f ? '+' : value < -0.055f ? '-' : '.';
            for (const auto& unit : world.getUnits())
                if (unit->getPos().getX() == static_cast<int>(x) &&
                    unit->getPos().getY() == static_cast<int>(y))
                    cell = unit->getTeam() == 0 ? 'A' : 'B';
            std::cout << cell;
        }
        std::cout << '\n';
    }
}

#else

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

#endif
