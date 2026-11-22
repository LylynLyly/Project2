#pragma once
#include "GridField.h"
class FieldSystem
{
private:
	GridField<float> influence;
public:
	void calculateInfluence(const World& w) {};
	const GridField<float>& getInfluence() const {return influence;}
	GridField<float> predictMove(const Unit& unit,const Math::Vector& newPosition) const;

};

