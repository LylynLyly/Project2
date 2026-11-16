#pragma once
class Unit
{
private:
	Math::Vector position;
	std::unique_ptr<BrainInterface> brain;
	double influence, utility;
public:
	Math::Vector GetPositon() { return position; }

};
