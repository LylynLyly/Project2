#include "World.h"
#include <algorithm>
#include <stdexcept>
#include <utility>

World::World(std::int8_t width, std::int8_t height) : width(width), height(height)
{}

void World::addUnit(std::unique_ptr<Unit> unit)
{
	if (!unit)
		throw std::invalid_argument("World::addUnit requires a unit");

	units.push_back(std::move(unit));
}

void World::removeDestroyedUnits()
{
	std::erase_if(units, [](const std::unique_ptr<Unit>& unit)
	{
		return unit->isDestroyed();
	});
}
