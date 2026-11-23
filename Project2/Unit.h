#pragma once
#include <vector>
#include "Vector.h"
class Unit
{
private:
	Math::Vector position;
	int team;
	double influence, utility, coef_a, coef_r, strength;
	double area, damage, armor_piercing;
	// [0] close range, [1] mid range, [2] long range
	std::vector<double> combatProfile;
public:
	Unit(Math::Vector position, int team, double strength,
		std::vector<double> combatProfile = { 1.0, 0.7, 0.3 },
		double area = 1.0, double damage = 1.0, double armorPiercing = 0.0);
	Math::Vector getPos() const;
	void setPos(Math::Vector newPosition);
	int getTeam() const;
	double getStrength() const;
	double getArea() const;
	double getDamage() const;
	double getArmorPiercing() const;
	bool isDestroyed() const;
	virtual void receiveDamage(double x);
	virtual void receiveAttack(double damage, double armorPiercing);
	double getCoefa() const;
	double getCoefr() const;
	void setCoefficients(double coefA, double coefR);
	const std::vector<double>& getCombatProfile() const;
	double getCombatCoefficient(double distance) const;
};
