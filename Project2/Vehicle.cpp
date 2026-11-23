#include "Vehicle.h"

Vehicle::Vehicle(Math::Vector position, int team, double strength, double armor,
	std::vector<double> combatProfile, double area, double damage, double armorPiercing)
	: Unit(position, team, strength, std::move(combatProfile), area, damage, armorPiercing), armor(armor)
{
}

void Vehicle::receiveDamage(double x)
{
	Unit::receiveDamage(x);
}

void Vehicle::receiveAttack(double damage, double armorPiercing)
{
	if (armorPiercing >= armor)
		receiveDamage(damage);
}
