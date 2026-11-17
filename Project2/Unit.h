#pragma once
class Unit
{
private:
	Math::Vector position;
	double influence, utility;
public:
	Math::Vector getPos() { return position; }

};
