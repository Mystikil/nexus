#ifndef FS_DUNGEON_GENERATOR_H
#define FS_DUNGEON_GENERATOR_H

#include "position.h"
#include "gate.h"
#include <vector>

enum class TileType;

class DungeonGenerator {
        public:
                explicit DungeonGenerator(GateRank rank);

                void generate();

                const std::vector<std::vector<TileType>>& getGrid() const { return grid; }
                const std::vector<Position>& getMonsterRelativePositions() const { return monsterPositions; }
                Position getEntranceRel() const { return entranceRel; }
                Position getExitRel() const { return exitRel; }

        private:
                GateRank rank;
                int width = 0;
                int height = 0;
                std::vector<std::vector<TileType>> grid;
                std::vector<Position> monsterPositions;
                Position entranceRel;
                Position exitRel;
};

#endif // FS_DUNGEON_GENERATOR_H

