#include "Personnel.h"

Personnel::Personnel(Math::Vector position, int team, double strength,
	std::vector<double> combatProfile, double area, double damage, double armorPiercing)
	: Unit(position, team, strength, std::move(combatProfile), area, damage, armorPiercing)
{
}

double Personnel::getControlStrength() const
{
	const double initialStrength = getInitialStrength();
	if (isDestroyed() || initialStrength <= 0.0)
		return 0.0;

	const double strength = getStrength();
	return strength * strength / initialStrength;
}
