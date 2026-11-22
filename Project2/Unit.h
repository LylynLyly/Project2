#pragma once
#include "Vector.h"
class Unit
{
private:
	Math::Vector position;
	int team;
	double influence, utility, coef_a, coef_r, strength;
public:
	Unit(Math::Vector position, int team, double strength);
	Math::Vector getPos() const;
	void setPos(Math::Vector newPosition);
	int getTeam() const;
	double getStrength() const;
	int getCoefa() const;
	int getCoefr() const;
};
