#ifndef FS_GATE_H
#define FS_GATE_H

#include "position.h"
#include <cstdint>

class Instance;

enum class GateRank : uint8_t {
	E = 0,
	D,
	C,
	B,
	A,
	S
};

enum class GateType : uint8_t {
	NORMAL = 0,
	RED,
	DOUBLE
};

class Gate {
	public:
		Gate() = default;

		void setId(uint32_t id) {
			this->id = id;
		}
		uint32_t getId() const {
			return id;
		}

		void setPosition(const Position& pos) {
			position = pos;
		}
		const Position& getPosition() const {
			return position;
		}

		void setCreationTime(int64_t time) {
			creationTime = time;
		}
		int64_t getCreationTime() const {
			return creationTime;
		}

		void setExpirationTime(int64_t time) {
			expirationTime = time;
		}
		int64_t getExpirationTime() const {
			return expirationTime;
		}

		void setRank(GateRank r) {
			rank = r;
		}
		GateRank getRank() const {
			return rank;
		}

		void setType(GateType t) {
			type = t;
		}
		GateType getType() const {
			return type;
		}

		void setInstance(Instance* inst) {
			instance = inst;
		}
		Instance* getInstance() const {
			return instance;
		}

		void setBossDefeated(bool val) {
			bossDefeated = val;
		}
		bool isBossDefeated() const {
			return bossDefeated;
		}

		void setExpired(bool val) {
			expired = val;
		}
		bool isExpired() const {
			return expired;
		}

		void setCleared(bool val) {
			cleared = val;
		}
		bool isCleared() const {
			return cleared;
		}

	private:
		uint32_t id = 0;
		Position position;
		int64_t creationTime = 0;
		int64_t expirationTime = 0;
		GateRank rank = GateRank::E;
		GateType type = GateType::NORMAL;
		Instance* instance = nullptr;
		bool bossDefeated = false;
		bool expired = false;
		bool cleared = false;
};

#endif // FS_GATE_H
