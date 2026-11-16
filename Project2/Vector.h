#pragma once
namespace Math
{
	class Vector
	{
	private:
		int x_, y_, z_;
	public:
		Vector(int x, int y, int z) : x_(x), y_(y), z_(z) {}
		int GetX() const { return x_; }
		int GetY() const { return y_; }
		int GetZ() const { return z_; }
		Vector operator+(const Vector& other) { return Vector(x_ + other.GetX(), y_ + other.GetY(), z_ + other.GetZ()); }
		Vector operator-(const Vector& other) { return Vector(x_ + other.GetX(), y_ + other.GetY(), z_ + other.GetZ()); }

	};
}