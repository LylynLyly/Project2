#pragma once
#include "GridField.h"
#include "LocalForces.h"

class Unit;
class World;
namespace Math
{
	class Vector;
}

class FieldSystem
{
private:
	GridField<float> influence;
public:
	FieldSystem(std::size_t width, std::size_t height);
	void calculateInfluence(const World& w);
	const GridField<float>& getInfluence() const {return influence;}
	GridField<float> predictMove(const Unit& unit,const Math::Vector& newPosition) const;
	LocalForces calculateForces(const World& world,const Unit& self,const Math::Vector& position) const;
};

