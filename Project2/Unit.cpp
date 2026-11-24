#include "Unit.h"
#include <algorithm>
#include <stdexcept>
#include <utility>

Unit::Unit(Math::Vector position, int team, double strength, std::vector<double> combatProfile, double area, double damage, double armorPiercing)
	: position(position), team(team), influence(0.0), utility(0.0), coef_a(1.0), coef_r(1.0),
	strength(strength), initialStrength(strength), area(area), damage(damage), armor_piercing(armorPiercing), combatProfile(std::move(combatProfile))
{
	if (this->combatProfile.size() != 3)
		throw std::invalid_argument("combatProfile must contain close, mid and long range coefficients");
}

Math::Vector Unit::getPos() const
{
	return position;
}

void Unit::setPos(Math::Vector newPosition)
{
	position = newPosition;
}

int Unit::getTeam() const
{
	return team;
}

double Unit::getStrength() const
{
	return strength;
}

double Unit::getInitialStrength() const
{
	return initialStrength;
}

double Unit::getControlStrength() const
{
	return getStrength();
}

double Unit::getArea() const { return area; }
double Unit::getDamage() const { return damage; }
double Unit::getArmorPiercing() const { return armor_piercing; }
bool Unit::isDestroyed() const { return strength <= 0.0; }
void Unit::receiveDamage(double x) { strength = std::max(0.0, strength - x); }
void Unit::receiveAttack(double damage, double armorPiercing) { receiveDamage(damage); }
double Unit::getCoefa() const { return coef_a; }
double Unit::getCoefr() const{ return coef_r; }

void Unit::setCoefficients(double coefA, double coefR)
{
	coef_a = coefA;
	coef_r = coefR;
}

const std::vector<double>& Unit::getCombatProfile() const { return combatProfile; }

double Unit::getCombatCoefficient(double distance) const
{
	if (distance <= 4.0)
		return combatProfile[0];
	if (distance <= 11.0)
		return combatProfile[1];
	return combatProfile[2];
}
