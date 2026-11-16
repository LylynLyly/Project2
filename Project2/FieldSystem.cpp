#include "FieldSystem.h"
void FieldSystem::calculateInfluence(const World& w) 
{
	for (size_t x=0; x < influence.width(); x++)
	{
		for (size_t y=0; y < influence.height(); y++)
		{
			float total = 0.0;
			for (const Unit& unit : w.getAgents())
			{
				float distance = (unit.getPos() - Math::Vector(x, y));
				float value = unit.getStrength() * gaussian(distance, 6.4);
				if (unit.GetTeam() == 0)
					total += value;
				else
					total -= value
			}
			influence.at(x,y) = influence;
		}
	}
}