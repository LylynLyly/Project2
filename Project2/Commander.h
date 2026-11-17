#pragma once
#include "GeometryStats .h"
#include "Vector.h"
#include "FieldSystem.h"
#include "Unit.h"
#include "GeometrySystem.h"
class Commander 
{
public:
    Math::Vector choosePosition(const Unit& unit, const FieldSystem& field, const GeometrySystem& analyzer);
};

