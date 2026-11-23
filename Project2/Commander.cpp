#include "Commander.h"
#include "GeometryStats.h"

ScoreBreakdown Commander::scorePosition(const World& world, const Unit& unit, const FieldSystem& field, const GeometrySystem& analyzer, const Math::Vector& position) const
{
    const auto before = analyzer.analyzeGeometry(unit.getTeam(), field.getInfluence());
    const auto predicted = field.predictMove(unit, position);
    const auto after = analyzer.analyzeGeometry(unit.getTeam(), predicted);

    const double area = unit.getCoefa() * static_cast<double>(after.area - before.area);
    const double risk = unit.getCoefr() * field.calculateForces(world, unit, position).balance();
    return { area, risk, area + risk };
}

Math::Vector Commander::choosePosition(const World& world,const Unit& unit,const FieldSystem& field,const GeometrySystem& analyzer)
{
    GeometryStats before = analyzer.analyzeGeometry(unit.getTeam(), field.getInfluence());
    Math::Vector bestPos = unit.getPos();
    float bestScore = 0.0f;
    const Math::Vector directions[] = 
    {
        { 1,  0 },
        { -1,  0},
        { 0,  1 },
        { 0, -1 }
    };
    for (const auto& direction : directions) 
    {
        Math::Vector candidate = unit.getPos() + direction;
        const auto& grid = field.getInfluence();
        if (candidate.getX() < 0 || candidate.getY() < 0 || static_cast<size_t>(candidate.getX()) >= grid.width() || static_cast<size_t>(candidate.getY()) >= grid.height())
        {
            continue;
        }

        auto predicted = field.predictMove(unit, candidate);
        GeometryStats after = analyzer.analyzeGeometry(unit.getTeam(), predicted);
        float score = unit.getCoefa() * static_cast<float>(after.area - before.area) + unit.getCoefr() * field.calculateForces(world,unit,candidate).balance();
        if (score > bestScore) 
        {
            bestScore = score;
            bestPos = candidate;
        }
    }
    return bestPos;
}
