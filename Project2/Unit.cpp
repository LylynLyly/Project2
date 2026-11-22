#include "Unit.h"

Unit::Unit(Math::Vector position, int team, double strength)
	: position(position), team(team), strength(strength)
{
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
