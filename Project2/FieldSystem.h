#pragma once
class FieldSystem
{
private:
	GridField<float> influence;
public:
	void calculateInfluence(const World& w) {};
	const GridField<float>& getInfluence() const {return influence;}
};

