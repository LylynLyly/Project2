#include "Vector.h"
#include <cmath>
namespace Math
{
    Vector::Vector(int x, int y) : x_(x), y_(y) {}
    Vector Vector::operator+(const Vector& other) const {return Vector(x_ + other.x_, y_ + other.y_);}
    Vector Vector::operator-(const Vector& other) const {return Vector(x_ - other.x_, y_ - other.y_);}
    int Vector::getX() const { return x_; }
    int Vector::getY() const { return y_; }
    double Vector::length() const {return std::hypot(x_, y_);}
}
