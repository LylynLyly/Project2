#include "World.h"
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
