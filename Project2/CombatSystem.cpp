#include "CombatSystem.h"
#include "Unit.h"
#include "World.h"

void CombatSystem::tick(const World& world, Unit& self) const
{
	if (self.isDestroyed())
		return;

	for (const auto& candidate : world.getUnits())
	{
		Unit& target = *candidate;
		if (&target == &self || target.getTeam() == self.getTeam() || target.isDestroyed())
			continue;

		const double distance = (self.getPos() - target.getPos()).length();
		if (distance > self.getArea())
			continue;

		const double attackDamage = self.getDamage() * self.getCombatCoefficient(distance);
		target.receiveAttack(attackDamage, self.getArmorPiercing());

		if (!target.isDestroyed() && distance <= target.getArea())
		{
			const double counterDamage = target.getDamage() * target.getCombatCoefficient(distance);
			self.receiveAttack(counterDamage, target.getArmorPiercing());
		}

		if (self.isDestroyed())
			return;
	}
}
