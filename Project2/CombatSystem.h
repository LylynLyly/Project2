#pragma once

class Unit;
class World;

class CombatSystem
{
public:
	void tick(const World& world, Unit& self) const;
};
