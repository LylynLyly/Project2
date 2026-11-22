#pragma once
#include "World.h"
#include "FieldSystem.h"
#include "GeometrySystem.h"
#include "Commander.h"
class Simulation
{
private:
    World& world;
    FieldSystem& field;
    GeometrySystem& geometry;
    Commander& commander;

public:
    Simulation(World& world,FieldSystem& field,GeometrySystem& geometry,Commander& commander);
    void tick();
};
