#include "otpch.h"

#include "dungeon_generator.h"
#include "instance.h"
#include "tools.h"

DungeonGenerator::DungeonGenerator(GateRank r) : rank(r)
{
	width = 10 + static_cast<int>(rank) * 2;
	height = width;
}

void DungeonGenerator::generate()
{
	// Initialize grid with FLOOR tiles
	grid.assign(height, std::vector<TileType>(width, TileType::FLOOR));

	// Create boundary walls
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			if (y == 0 || x == 0 || y == height - 1 || x == width - 1) {
				grid[y][x] = TileType::WALL;
			}
		}
	}

	// Set entrance and exit positions (relative to instance base)
	entranceRel = Position(1, 1, 0);
	exitRel = Position(width - 2, height - 2, 0);

	// Randomly place monsters
	monsterPositions.clear();
	int monsterCount = static_cast<int>(rank) + 1;

	std::uniform_int_distribution<int> distX(2, width - 3);
	std::uniform_int_distribution<int> distY(2, height - 3);

	for (int i = 0; i < monsterCount; ++i) {
		monsterPositions.emplace_back(distX(getRandomGenerator()), distY(getRandomGenerator()), 0);
	}
}
