#pragma once
struct LocalForces
{
    float allies = 0.0f;
    float enemies = 0.0f;

    float balance() const { return allies - enemies; }
};