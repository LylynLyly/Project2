#pragma once
#include "Vector.h"
#include "FieldSystem.h"
#include "Unit.h"
#include "GeometrySystem.h"

class World;

struct ScoreBreakdown
{
    double area = 0.0;
    double risk = 0.0;
    double total = 0.0;
};

class Commander 
{
public:
    Math::Vector choosePosition(const World& world, const Unit& unit, const FieldSystem& field, const GeometrySystem& analyzer);
    ScoreBreakdown scorePosition(const World& world, const Unit& unit, const FieldSystem& field, const GeometrySystem& analyzer, const Math::Vector& position) const;
};

