#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include "Commander.h"
#include "CombatSystem.h"
#include "FieldSystem.h"
#include "GeometrySystem.h"
#include "Simulation.h"
#include "Unit.h"
#include "Personnel.h"
#include "Vehicle.h"
#include "World.h"

namespace
{
    void renderFrame(int step, const World& world, const FieldSystem& field,
        const GeometryStats& firstTeam, const GeometryStats& secondTeam,
        double coefA, double coefR)
    {
        std::cout << "Step " << step + 1 << "/60: team 0 area=" << firstTeam.area
            << ", team 1 area=" << secondTeam.area
            << ", coefA=" << coefA << ", coefR=" << coefR << '\n';
        std::cout << "A=team 0 personnel, V=team 0 vehicle, B=team 1 personnel, "
            "'+/-'=influence\n";

        for (std::size_t y = 0; y < field.getInfluence().height(); ++y)
        {
            for (std::size_t x = 0; x < field.getInfluence().width(); ++x)
            {
                const float value = field.getInfluence().at(x, y);
                char cell = value > 0.055f ? '+' : value < -0.055f ? '-' : '.';
                for (const auto& unit : world.getUnits())
                {
                    if (unit->getPos().getX() == static_cast<int>(x) &&
                        unit->getPos().getY() == static_cast<int>(y))
                    {
                        cell = unit->getTeam() == 0
                            ? (dynamic_cast<const Vehicle*>(unit.get()) ? 'V' : 'A')
                            : 'B';
                    }
                }
                std::cout << cell;
            }
            std::cout << '\n';
        }
        std::cout.flush();
    }
}

int main()
{
    World world(60, 60);
    
    world.addUnit(std::make_unique<Personnel>(
        Math::Vector(3, 8), 0, 6.0, std::vector<double>{ 1.6, 0.0, 0.2 }, 5.0, 1.0, 0.5));
    world.addUnit(std::make_unique<Personnel>(
        Math::Vector(7, 22), 0, 6.0, std::vector<double>{ 1.6, 0.0, 0.2 }, 5.0, 1.0, 0.5));
    world.addUnit(std::make_unique<Vehicle>(
        Math::Vector(12, 15), 0, 12.0, 2.0, std::vector<double>{ 1.2, 1.0, 0.6 }, 7.0, 2.0, 1.0));

    world.addUnit(std::make_unique<Personnel>(
        Math::Vector(18, 10), 1, 6.0, std::vector<double>{ 0.4, 0.0, 1.5 }, 5.0, 1.5, 1.2));
    world.addUnit(std::make_unique<Personnel>(
        Math::Vector(24, 20), 1, 6.0, std::vector<double>{ 0.4, 0.0, 1.5 }, 5.0, 1.5, 1.2));
    world.addUnit(std::make_unique<Personnel>(
        Math::Vector(30, 12), 1, 6.0, std::vector<double>{ 0.4, 0.0, 1.5 }, 5.0, 1.5, 1.2));
    world.addUnit(std::make_unique<Personnel>(
        Math::Vector(35, 24), 1, 6.0, std::vector<double>{ 0.4, 0.0, 1.5 }, 5.0, 1.5, 1.2));
    world.addUnit(std::make_unique<Personnel>(
        Math::Vector(42, 16), 1, 6.0, std::vector<double>{ 0.4, 0.0, 1.5 }, 5.0, 1.5, 1.2));

    FieldSystem field(60, 60);
    GeometrySystem geometry;
    Commander commander;
    CombatSystem combat;
    Simulation simulation(world, field, geometry, commander, combat);

    for (int step = 0; step < 60; ++step)
    {
        
        const double coefA = step < 3 ? 12.0 : step < 6 ? 0.05 : 1.0;
        const double coefR = step < 3 ? 0.05 : step < 6 ? 12.0 : 1.0;
        for (const auto& unit : world.getUnits())
            unit->setCoefficients(coefA, coefR);

        simulation.tick();
        const auto firstTeam = geometry.analyzeGeometry(0, field.getInfluence());
        const auto secondTeam = geometry.analyzeGeometry(1, field.getInfluence());
        renderFrame(step, world, field, firstTeam, secondTeam, coefA, coefR);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}
