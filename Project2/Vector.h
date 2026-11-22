#pragma once
namespace Math
{
    class Vector
    {
    private:
        int x_, y_;

    public:
        Vector(int x, int y);
        Vector operator+(const Vector& other) const;
        Vector operator-(const Vector& other) const;
        int getX() const;
        int getY() const;
        double length() const;
    };
}
