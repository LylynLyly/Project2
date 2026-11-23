#pragma once
#include <vector>
#include <memory>
#include <cstdint>
#include "Unit.h"
class World
{
private:
	std::int8_t width{}, height{};
	std::vector<std::unique_ptr<Unit>> units;
public:
	World() = default;
	World(std::int8_t width, std::int8_t height);
	void addUnit(std::unique_ptr<Unit> unit);
	void removeDestroyedUnits();
	const std::vector<std::unique_ptr<Unit>>& getUnits() const { return units; }
};

