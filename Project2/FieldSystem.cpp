#include "FieldSystem.h"
#include "Vector.h"
#include "Unit.h"
#include "World.h"

double gaussian(double distance, double scale);

FieldSystem::FieldSystem(std::size_t width, std::size_t height) : influence(width, height, 0.0f) { }

void FieldSystem::calculateInfluence(const World& w) 
{
	for (std::size_t x=0; x < influence.width(); x++)
	{
		for (std::size_t y=0; y < influence.height(); y++)
		{
			float total = 0.0;
			for (const auto& unit : w.getUnits())
			{
				float distance = static_cast<float>(
					(unit->getPos() - Math::Vector(static_cast<int>(x), static_cast<int>(y))).length());
				float value = static_cast<float>(unit->getControlStrength() * gaussian(distance, 6.4));
				if (unit->getTeam() == 0)
					total += value;
				else
					total -= value;
			}
			influence.at(x,y) = total;
		}
	}
}

GridField<float> FieldSystem::predictMove(const Unit& unit, const Math::Vector& newPosition) const
{
    GridField<float> predicted = influence;
    float sign = (unit.getTeam() == 0) ? 1.0f : -1.0f;
    float strength = static_cast<float>(unit.getControlStrength() * sign);
    for (std::size_t y = 0; y < predicted.height(); ++y)
    {
        for (std::size_t x = 0; x < predicted.width(); ++x)
        {
            Math::Vector point(static_cast<int>(x), static_cast<int>(y));
            float oldDistance = static_cast<float>((unit.getPos() - point).length());
            float newDistance = static_cast<float>((newPosition - point).length());
            float oldInfluence = static_cast<float>(strength * gaussian(oldDistance, 6.4f));
            float newInfluence = static_cast<float>(strength * gaussian(newDistance, 6.4f));
            predicted.at(x, y) += newInfluence - oldInfluence;
        }
    }

    return predicted;
}

LocalForces FieldSystem::calculateForces(const World& world, const Unit& self, const Math::Vector& position) const
{
	LocalForces result;
	result.allies = static_cast<float>(self.getStrength() * self.getCombatCoefficient(0.0));
	for (const auto& other : world.getUnits())
	{
		if (other.get() == &self)
			continue;
		double distance = (other->getPos() - position).length();
		float contribution = static_cast<float>(other->getStrength() * other->getCombatCoefficient(distance) * gaussian(distance, 6.4f));
		if (other->getTeam() == self.getTeam())
			result.allies += contribution;
		else
			result.enemies += contribution;
	}
	return result;
}
