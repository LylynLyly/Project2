#pragma once
#include "Unit.h"

class Vehicle : public Unit
{
private:
	double armor;

public:
	Vehicle(Math::Vector position, int team, double strength, double armor,
		std::vector<double> combatProfile = { 1.0, 0.7, 0.3 },
		double area = 1.0, double damage = 1.0, double armorPiercing = 0.0);
	void receiveDamage(double x) override;
	void receiveAttack(double damage, double armorPiercing) override;
};

