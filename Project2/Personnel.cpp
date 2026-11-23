#include "Personnel.h"

Personnel::Personnel(Math::Vector position, int team, double strength,
	std::vector<double> combatProfile, double area, double damage, double armorPiercing)
	: Unit(position, team, strength, std::move(combatProfile), area, damage, armorPiercing)
{
}
