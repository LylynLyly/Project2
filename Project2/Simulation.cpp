#include "Simulation.h"
#include <cstddef>

Simulation::Simulation(World& world, FieldSystem& field, GeometrySystem& geometry, Commander& commander, CombatSystem& combat)
	: world(world), field(field), geometry(geometry), commander(commander), combat(combat) { }

void Simulation::tick()
{
	field.calculateInfluence(world);
	for (const auto& unit : world.getUnits())
	{
		if (unit->isDestroyed())
			continue;
		unit->setPos(commander.choosePosition(world, *unit, field, geometry));
		combat.tick(world, *unit);
		field.calculateInfluence(world);
	}

	world.removeDestroyedUnits();
	field.calculateInfluence(world);
}
