#pragma once
#include "Vector.h"
class Unit
{
private:
	Math::Vector position;
	int8_t team,s;
	double influence, utility;
public:
	Math::Vector getPos() const { return position; }
	int8_t getTeam() const { return team; }
	int8_t getStrength() const { return s; }
};
