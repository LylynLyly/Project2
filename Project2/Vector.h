#pragma once
#include <cmath>
namespace Math
{
    class Vector
    {
    private:
        int x_, y_;

    public:
        Vector(int x, int y);
        Vector operator+(const Vector& other);
        Vector operator-(const Vector& other);
        double length();
    };
}