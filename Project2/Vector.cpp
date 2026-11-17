#include "Vector.h"
namespace Math
{
    Vector::Vector(int x, int y) : x_(x), y_(y) {}
    Vector Vector::operator+(const Vector& other) {return Vector(x_ + other.x_, y_ + other.y_);}
    Vector Vector::operator-(const Vector& other) {return Vector(x_ - other.x_, y_ - other.y_);}
    double Vector::length() {return std::hypot(x_, y_);}
}