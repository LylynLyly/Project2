#pragma once
#include "Unit.h"

class Personnel : public Unit
{
public:
	Personnel(Math::Vector position, int team, double strength,
		std::vector<double> combatProfile = { 1.0, 0.7, 0.3 },
		double area = 1.0, double damage = 1.0, double armorPiercing = 0.0);
};

