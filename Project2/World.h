#pragma once
#include <vector>
#include <memory>
#include "Unit.h"
class World
{
private:
	__int8 witdh, height;
	std::vector<std::unique_ptr<Unit>> units;
public:
	std::vector<std::unique_ptr<Unit>> getUnits() const { return units; }
};

