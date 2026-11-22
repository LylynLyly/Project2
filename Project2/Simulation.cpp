#include "Simulation.h"
#include <cstddef>

Simulation::Simulation(World& world, FieldSystem& field, GeometrySystem& geometry, Commander& commander)
    : world(world), field(field), geometry(geometry), commander(commander)
{
}

void Simulation::tick()
{
	field.calculateInfluence(world);
	for (const auto& unit : world.getUnits())
	{
		unit->setPos(commander.choosePosition(*unit, field, geometry));
		field.calculateInfluence(world);
	}
}
