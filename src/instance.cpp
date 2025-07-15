#include "otpch.h"

#include "instance.h"
#include "dungeon_generator.h"
#include "events.h"
#include "game.h"
#include "monster.h"
#include "tile.h"

extern Game g_game;

Instance::Instance(uint32_t id, GateRank r) : instanceId(id), rank(r)
{
        originPosition = Position(31000 + static_cast<uint16_t>(id * 100), 31000, 7);
}

Instance::~Instance()
{
        cleanup();
}

void Instance::generateLayout()
{
        DungeonGenerator generator(rank);
        generator.generate();

        grid = generator.getGrid();

        Position entranceRel = generator.getEntranceRel();
        entryPoint = Position(originPosition.x + entranceRel.x, originPosition.y + entranceRel.y, originPosition.z);

        Position exitRel = generator.getExitRel();
        exitPoints.clear();
        exitPoints.emplace_back(originPosition.x + exitRel.x, originPosition.y + exitRel.y, originPosition.z);

        monsterSpawns.clear();
        for (const Position& rel : generator.getMonsterRelativePositions()) {
                monsterSpawns.emplace_back(originPosition.x + rel.x, originPosition.y + rel.y, originPosition.z);
        }
}

void Instance::placeTiles()
{
        for (size_t y = 0; y < grid.size(); ++y) {
                for (size_t x = 0; x < grid[y].size(); ++x) {
                        TileType type = grid[y][x];
                        if (type == TileType::EMPTY) {
                                continue;
                        }

                        Position pos(originPosition.x + x, originPosition.y + y, originPosition.z);
                        Tile* tile = g_game.map.getTile(pos);
                        if (!tile) {
                                tile = new DynamicTile(pos.x, pos.y, pos.z);
                                g_game.map.setTile(pos, tile);
                                createdTiles.push_back(pos);
                        }
                }
        }
}

void Instance::spawnMonsters()
{
        static const char* MONSTERS[] = {"rat", "orc", "cyclops", "dragon", "hydra", "demon"};
        const char* monsterName = MONSTERS[std::min<size_t>(static_cast<size_t>(rank), 5)];
        for (const Position& pos : monsterSpawns) {
                Monster* monster = Monster::createMonster(monsterName);
                if (!monster) {
                        continue;
                }

                if (events::monster::onSpawn(monster, pos, false, true)) {
                        if (!g_game.placeCreature(monster, pos, false, true)) {
                                delete monster;
                        } else {
                                spawnedMonsters.push_back(monster);
                        }
                } else {
                        delete monster;
                }
        }
}

void Instance::cleanup()
{
        for (Monster* monster : spawnedMonsters) {
                if (monster && !monster->isRemoved()) {
                        g_game.removeCreature(monster, false);
                }
        }
        spawnedMonsters.clear();

        for (const Position& pos : createdTiles) {
                g_game.map.removeTile(pos);
        }
        createdTiles.clear();
}

