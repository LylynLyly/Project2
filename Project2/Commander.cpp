#include "Commander.h"
#include <vector>
#include "GeometryStats .h"
#include "Vector.h"
Math::Vector Commander::choosePosition(const Unit& unit,const FieldSystem& field,const GeometrySystem& analyzer)
{
    GeometryStats before = analyzer.analyze(field.getInfluence(),unit.GetTeam());
    Math::Vector bestPos = unit.getPos();
    float bestScore = 0.0f;
    const Math::Vector directions[] = 
    {
        { 1,  0 },
        { -1,  0 },
        { 0,  1 },
        { 0, -1 }
    };
    for (const auto& direction : directions) 
    {
        Math::Vector candidate = unit.getPos() + direction;
        auto predicted = field.predictMove(unit, candidate);
        GeometryStats after = analyzer.analyzeGeometry(predicted,unit.GetTeam());
        float score = static_cast<float>(after.area - before.area);
        if (score > bestScore) 
        {
            bestScore = score;
            bestPos = candidate;
        }
    }
    return bestPos;
}