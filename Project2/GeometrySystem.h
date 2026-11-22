#pragma once
#include "GeometryStats.h"
#include "GridField.h"
class GeometrySystem
{
public:
	GeometryStats analyzeGeometry(int team, const GridField<float>& influence) const;
};

