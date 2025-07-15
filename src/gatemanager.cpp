// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "gatemanager.h"

#include <iostream>

Gate* GateManager::spawnGate(const Position& pos, GateRank rank, GateType type)
{
	uint32_t id = generateGateId();

	auto [it, inserted] = gates.emplace(id, Gate{});
	Gate& gate = it->second;
	gate.setId(id);
	gate.setPosition(pos);
	gate.setRank(rank);
	gate.setType(type);

	int64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::steady_clock::now().time_since_epoch())
			.count();
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
		// TODO: call Lua hook onGateBreak(gate)
		// TODO: spawn break mobs at gate position
		} else {
		std::cout << "[GateManager] Gate " << id << " expired." << std::endl;
		}
		removeList.push_back(id);
	}
	}

	for (uint32_t id : removeList) {
	gates.erase(id);
	}
}

void GateManager::removeGate(uint32_t gateId)
{
	gates.erase(gateId);
}

uint32_t GateManager::generateGateId()
{
	static std::atomic<uint32_t> lastId{0};
	return ++lastId;
}

