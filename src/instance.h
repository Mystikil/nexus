#ifndef FS_INSTANCE_H
#define FS_INSTANCE_H

#include "position.h"
#include "gate.h"

#include <vector>

class Monster;

enum class TileType {
        EMPTY,
        FLOOR,
        WALL,
        MONSTER,
        ENTRANCE,
        EXIT
};

class Instance {
        public:
                Instance(uint32_t instanceId, GateRank rank);
                ~Instance();

                void generateLayout();
                void placeTiles();
                void spawnMonsters();
                void cleanup();

                uint32_t getId() const { return instanceId; }
                const Position& getOriginPosition() const { return originPosition; }
                const Position& getEntryPoint() const { return entryPoint; }

        private:
                uint32_t instanceId = 0;
                Position originPosition;
                Position entryPoint;
                std::vector<Position> monsterSpawns;
                std::vector<Position> exitPoints;
                GateRank rank;

                std::vector<std::vector<TileType>> grid;
                std::vector<Position> createdTiles;
                std::vector<Monster*> spawnedMonsters;
};

#endif // FS_INSTANCE_H

