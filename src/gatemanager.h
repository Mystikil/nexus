#ifndef FS_GATEMANAGER_H
#define FS_GATEMANAGER_H

#include "gate.h"
#include <map>
#include <vector>
#include <cstdint>

struct SpawnRule {
       GateRank rank;
       GateType type;
       uint32_t maxCount;
};

class GateManager {
        public:
                Gate* spawnGate(const Position& pos, GateRank rank, GateType type);
                Gate* getGate(uint32_t gateId);
                void update();
                void removeGate(uint32_t gateId);
               void loadSpawnConfig(const std::string& file);

        private:
                uint32_t generateGateId();
                std::map<uint32_t, Gate> gates;
               Position spawnCenter;
               uint32_t spawnRadius = 25;
               uint32_t spawnIntervalMs = 60000;
               std::vector<SpawnRule> spawnRules;
               int64_t lastSpawn = 0;
};

extern GateManager g_gateManager;

#endif // FS_GATEMANAGER_H
