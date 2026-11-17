#include "FieldSystem.h"
#include <vector>
#include <queue>
#include <cstdint>
#include <stdexcept>
void FieldSystem::calculateInfluence(const World& w) 
{
	for (size_t x=0; x < influence.width(); x++)
	{
		for (size_t y=0; y < influence.height(); y++)
		{
			float total = 0.0;
			for (const Unit& unit : w.getAgents())
			{
				float distance = (unit.getPos() - Math::Vector(x, y));
				float value = unit.getStrength() * gaussian(distance, 6.4);
				if (unit.GetTeam() == 0)
					total += value;
				else
					total -= value
			}
			influence.at(x,y) = influence;
		}
	}
}
GeometryStats FieldSystem::analyzeGeometry(int team)
{
	GeometryStats stats;
	const int W = static_cast<int>(influence.width());
	const int H = static_cast<int>(influence.height());
	auto index = [W](int x, int y) {return y * W + x;};
	std::vector<uint8_t> mask(W * H, 0);
	std::vector<uint8_t> visited(W * H, 0);
	std::vector<uint8_t> outside(W * H, 0);
	for (int x = 0; x < W; x++)
	{
		for (int y = 0; y < H; y++)
		{
			float value = influence.at(x, y);
			bool occupied = (team == 0) ? value > 0.055f : value < -0.055f;
			mask[index(x, y)] = occupied ? 1 : 0;
			if (occupied)
				++stats.area;
		}
	}
	const int dx[4] = { 1, -1, 0, 0 };
	const int dy[4] = { 0, 0, 1, -1 };
	auto floodFill = [&](int sx, int sy, uint8_t target, std::vector<uint8_t>& visited)
		{
			std::queue<std::pair<int, int>> q;
			q.push({ sx, sy });
			visited[index(sx, sy)] = 1;
			while (!q.empty())
			{
				auto [x, y] = q.front();
				q.pop();
				for (int d = 0; d < 4; ++d)
				{
					int nx = x + dx[d];
					int ny = y + dy[d];

					if (nx < 0 || nx >= W || ny < 0 || ny >= H)
						continue;

					int id = index(nx, ny);

					if (!visited[id] && mask[id] == target)
					{
						visited[id] = 1;
						q.push({ nx, ny });
					}
				}
			}
		};
	for (int y = 0; y < H; ++y) 
	{
		for (int x = 0; x < W; ++x) 
		{
			int id = index(x, y);
			if (mask[id] && !visited[id]) 
			{
				++stats.components;
				floodFill(x, y, 1, visited);
			}
		}
	}
	for (int y = 0; y < H; ++y) 
	{
		for (int x = 0; x < W; ++x) 
		{
			bool boundary = (x == 0 || x == W - 1 || y == 0 || y == H - 1);
			int id = index(x, y);
			if (boundary && !mask[id] && !outside[id])
				floodFill(x, y, 0, outside);
		}
	}
	for (int y = 0; y < H; ++y) 
	{
		for (int x = 0; x < W; ++x) 
		{
			int id = index(x, y);
			if (!mask[id] && !outside[id]) 
			{
				++stats.holes;
				floodFill(x, y, 0, outside);
			}
		}
	}
	return stats;
}