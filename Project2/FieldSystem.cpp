#include "FieldSystem.h"
#include <vector>
#include <queue>
#include <cstdint>
#include <stdexcept>
#include "GeometryStats.h"
#include "Vector.h"
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
GridField<float> FieldSystem::predictMove(const Unit& unit,const Math::Vector& newPosition) 
{
    GridField<float> predicted = influence;
    float sign = (unit.GetTeam() == 0) ? 1.0f : -1.0f;
    float strength = unit.getStrength() * sign;
    for (int y = 0; y < predicted.height(); ++y) 
    {
        for (int x = 0; x < predicted.width(); ++x) 
        {
            Math::Vector point(x, y);
            float oldDistance = (unit.getPos() - point).length();
            float newDistance = (newPosition - point).length();
            float oldInfluence = strength * gaussian(oldDistance, 6.4f);
            float newInfluence = strength * gaussian(newDistance, 6.4f);
            predicted.at(x, y) += newInfluence - oldInfluence;
        }
    }

    return predicted;
}