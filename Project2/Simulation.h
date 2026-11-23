#pragma once
#include "World.h"
#include "FieldSystem.h"
#include "GeometrySystem.h"
#include "Commander.h"
#include "CombatSystem.h"
class Simulation
{
private:
    World& world;
    FieldSystem& field;
    GeometrySystem& geometry;
    Commander& commander;
	CombatSystem& combat;

public:
    Simulation(World& world, FieldSystem& field, GeometrySystem& geometry, Commander& commander, CombatSystem& combat);
    void tick();
};
