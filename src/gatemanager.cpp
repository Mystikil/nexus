// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "gatemanager.h"

#include <iostream>

#include "game.h"
#include "instance.h"
#include "teleport.h"
#include "luascript.h"
#include "monster.h"
#include "events.h"

extern Game g_game;
extern LuaEnvironment g_luaEnvironment;

namespace {
       const char* RANK_NAMES[] = {"E", "D", "C", "B", "A", "S"};

       void spawnBreakWave(const Gate& gate)
       {
               lua_State* L = g_luaEnvironment.getLuaState();
               lua_getglobal(L, "GateBreakWaves");
               if (!lua_istable(L, -1)) {
                       lua_pop(L, 1);
                       return;
               }

               lua_getfield(L, -1, RANK_NAMES[static_cast<uint8_t>(gate.getRank())]);
               if (!lua_istable(L, -1)) {
                       lua_pop(L, 2);
                       return;
               }

               lua_pushnil(L);
               int offset = 0;
               while (lua_next(L, -2) != 0) {
                       const auto tableIndex = lua_gettop(L);
                       std::string monsterName = lua::getFieldString(L, tableIndex, "name");
                       int count = lua::getField<int>(L, tableIndex, "count", 1);
                       lua_pop(L, 1); // pop value, keep key

                       for (int i = 0; i < count; ++i) {
                               Monster* monster = Monster::createMonster(monsterName);
                               if (!monster) {
                                       continue;
                               }

                               Position spawnPos = gate.getPosition();
                               spawnPos.x += offset++;

                               if (events::monster::onSpawn(monster, spawnPos, false, true)) {
                                       if (!g_game.placeCreature(monster, spawnPos, false, true)) {
                                               delete monster;
                                       }
                               } else {
                                       delete monster;
                               }
                       }
               }

               lua_pop(L, 2); // rank table and GateBreakWaves
       }
}

Gate* GateManager::spawnGate(const Position& pos, GateRank rank, GateType type)
{
	uint32_t id = generateGateId();

	auto [it, inserted] = gates.emplace(id, Gate{});
	Gate& gate = it->second;
	gate.setId(id);
	gate.setPosition(pos);
	gate.setRank(rank);
	gate.setType(type);

	// Create dungeon instance if applicable
	Instance* instance = nullptr;
	if (type == GateType::NORMAL || type == GateType::RED || type == GateType::DOUBLE) {
		instance = new Instance(id, rank);
		instance->generateLayout();
		instance->placeTiles();
		instance->spawnMonsters();
		gate.setInstance(instance);
		std::cout << "[GateManager] Instance attached to gate " << id << std::endl;
	}

	// Create teleport tile at gate position
	Tile* gateTile = g_game.map.getTile(pos);
	if (!gateTile) {
		gateTile = new DynamicTile(pos.x, pos.y, pos.z);
		g_game.map.setTile(pos, gateTile);
	}

	// Create teleport item and set its destination
	Teleport* tp = new Teleport(1387);
	if (instance) {
		tp->setDestPos(instance->getEntryPoint());
	} else {
		tp->setDestPos(pos); // fallback if instance is null
	}

	g_game.internalAddItem(gateTile, tp, INDEX_WHEREEVER, FLAG_NOLIMIT);

	// Set timing values
	int64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::steady_clock::now().time_since_epoch()).count();
	gate.setCreationTime(now);

	uint32_t minutes = (static_cast<uint8_t>(rank) + 1) * 10; // 10-60 minutes depending on rank
	gate.setExpirationTime(now + static_cast<int64_t>(minutes) * 60 * 1000);

	std::cout << "[GateManager] Spawned gate " << id << " at (" << pos.getX() << ','
		<< pos.getY() << ',' << static_cast<int>(pos.getZ()) << ")" << std::endl;

	return &gate;
}


Gate* GateManager::getGate(uint32_t gateId)
{
	auto it = gates.find(gateId);
	if (it == gates.end()) {
	return nullptr;
	}
	return &it->second;
}

void GateManager::update()
{
	int64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::steady_clock::now().time_since_epoch())
			.count();

	std::vector<uint32_t> removeList;
	for (auto& [id, gate] : gates) {
	if (!gate.isExpired() && now >= gate.getExpirationTime()) {
		gate.setExpired(true);
		if (!gate.isCleared()) {
                std::cout << "[GateManager] Gate " << id << " has broken!" << std::endl;

                lua_State* L = g_luaEnvironment.getLuaState();
                lua_getglobal(L, "onGateBreak");
                if (lua_isfunction(L, -1)) {
                        if (lua::reserveScriptEnv()) {
                                ScriptEnvironment* env = lua::getScriptEnv();
                                env->setScriptId(-1, &g_luaEnvironment);

                                lua_newtable(L);
                                lua_pushnumber(L, gate.getId());
                                lua_setfield(L, -2, "id");
                                lua::pushPosition(L, gate.getPosition());
                                lua_setfield(L, -2, "position");
                                lua_pushnumber(L, static_cast<int>(gate.getRank()));
                                lua_setfield(L, -2, "rank");
                                lua_pushnumber(L, static_cast<int>(gate.getType()));
                                lua_setfield(L, -2, "type");

                                if (lua_pcall(L, 1, 0, 0) != 0) {
                                        std::cout << "[Warning - GateManager::update] onGateBreak: " << lua_tostring(L, -1) << std::endl;
                                        lua_pop(L, 1);
                                }
                                lua::resetScriptEnv();
                        } else {
                                lua_pop(L, 1);
                        }
                } else {
                        lua_pop(L, 1);
                }

                spawnBreakWave(gate);
		} else {
		std::cout << "[GateManager] Gate " << id << " expired." << std::endl;
		}
		removeList.push_back(id);
	}
	}

        for (uint32_t id : removeList) {
                auto it = gates.find(id);
                if (it != gates.end()) {
                        Instance* inst = it->second.getInstance();
                        if (inst) {
                                inst->cleanup();
                                delete inst;
                                it->second.setInstance(nullptr);
                        }
                        g_game.map.removeTile(it->second.getPosition());
                        gates.erase(it);
                }
        }
}

void GateManager::removeGate(uint32_t gateId)
{
        auto it = gates.find(gateId);
        if (it != gates.end()) {
                Instance* inst = it->second.getInstance();
                if (inst) {
                        inst->cleanup();
                        delete inst;
                }
                g_game.map.removeTile(it->second.getPosition());
                gates.erase(it);
        }
}

uint32_t GateManager::generateGateId()
{
	static std::atomic<uint32_t> lastId{0};
	return ++lastId;
}

