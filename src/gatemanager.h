#ifndef FS_GATEMANAGER_H
#define FS_GATEMANAGER_H

#include "gate.h"
#include <map>
#include <cstdint>

class GateManager {
	public:
		Gate* spawnGate(const Position& pos, GateRank rank, GateType type);
		Gate* getGate(uint32_t gateId);
		void update();
		void removeGate(uint32_t gateId);

	private:
		uint32_t generateGateId();
		std::map<uint32_t, Gate> gates;
};

extern GateManager g_gateManager;

#endif // FS_GATEMANAGER_H
