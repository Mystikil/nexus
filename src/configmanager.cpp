// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "configmanager.h"
#include "game.h"
#include "monster.h"
#include "pugicast.h"

#if __has_include("luajit/lua.hpp")
#include <luajit/lua.hpp>
#else
#include <lua.hpp>
#endif

#if LUA_VERSION_NUM >= 502
#undef lua_strlen
#define lua_strlen lua_rawlen
#endif

extern Game g_game;

namespace {

        std::array<std::string, ConfigManager::LAST_STRING_CONFIG> string = {};
        std::array<int32_t, ConfigManager::LAST_INTEGER_CONFIG> integer = {};
        std::array<bool, ConfigManager::LAST_BOOLEAN_CONFIG> boolean = {};
        std::array<float, ConfigManager::LAST_FLOAT_CONFIG> floats = {};

	using ExperienceStages = std::vector<std::tuple<uint32_t, uint32_t, float>>;
        ExperienceStages expStages;
        std::vector<ConfigManager::MonsterLevelRule> monsterLevelRules;

	bool loaded = false;

	std::string getGlobalString(lua_State* L, const char* identifier, const char* defaultValue) {
		lua_getglobal(L, identifier);
		if (!lua_isstring(L, -1)) {
			lua_pop(L, 1);
			return defaultValue;
		}

		size_t len = lua_strlen(L, -1);
		std::string ret(lua_tostring(L, -1), len);
		lua_pop(L, 1);
		return ret;
	}

	int32_t getGlobalNumber(lua_State* L, const char* identifier, const int32_t defaultValue = 0) {
		lua_getglobal(L, identifier);
		if (!lua_isnumber(L, -1)) {
			lua_pop(L, 1);
			return defaultValue;
		}

		int32_t val = lua_tonumber(L, -1);
		lua_pop(L, 1);
		return val;
	}

        bool getGlobalBoolean(lua_State* L, const char* identifier, const bool defaultValue) {
                lua_getglobal(L, identifier);
                if (!lua_isboolean(L, -1)) {
                        if (!lua_isstring(L, -1)) {
                                lua_pop(L, 1);
                                return defaultValue;
                        }

			size_t len = lua_strlen(L, -1);
			std::string ret(lua_tostring(L, -1), len);
			lua_pop(L, 1);
			return booleanString(ret);
		}

                int val = lua_toboolean(L, -1);
                lua_pop(L, 1);
                return val != 0;
        }

        float getGlobalFloat(lua_State* L, const char* identifier, float defaultValue = 0.f) {
                lua_getglobal(L, identifier);
                if (!lua_isnumber(L, -1)) {
                        lua_pop(L, 1);
                        return defaultValue;
                }

                float val = lua_tonumber(L, -1);
                lua_pop(L, 1);
                return val;
        }

	ExperienceStages loadLuaStages(lua_State* L) {
		ExperienceStages stages;

		lua_getglobal(L, "experienceStages");
		if (!lua_istable(L, -1)) {
			return {};
		}

		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			const auto tableIndex = lua_gettop(L);
			auto minLevel = lua::getField<uint32_t>(L, tableIndex, "minlevel", 1);
			auto maxLevel = lua::getField<uint32_t>(L, tableIndex, "maxlevel", std::numeric_limits<uint32_t>::max());
			auto multiplier = lua::getField<float>(L, tableIndex, "multiplier", 1);
			stages.emplace_back(minLevel, maxLevel, multiplier);
			lua_pop(L, 4);
		}
		lua_pop(L, 1);

		std::sort(stages.begin(), stages.end());
		return stages;
	}

        ExperienceStages loadXMLStages() {
                pugi::xml_document doc;
                pugi::xml_parse_result result = doc.load_file("data/XML/stages.xml");
		if (!result) {
			printXMLError("Error - loadXMLStages", "data/XML/stages.xml", result);
			return {};
		}

		ExperienceStages stages;
		for (auto stageNode : doc.child("stages").children()) {
			if (caseInsensitiveEqual(stageNode.name(), "config")) {
				if (!stageNode.attribute("enabled").as_bool()) {
					return {};
				}
			} else {
				uint32_t minLevel = 1, maxLevel = std::numeric_limits<uint32_t>::max(), multiplier = 1;

				if (auto minLevelAttribute = stageNode.attribute("minlevel")) {
					minLevel = pugi::cast<uint32_t>(minLevelAttribute.value());
				}

				if (auto maxLevelAttribute = stageNode.attribute("maxlevel")) {
					maxLevel = pugi::cast<uint32_t>(maxLevelAttribute.value());
				}

				if (auto multiplierAttribute = stageNode.attribute("multiplier")) {
					multiplier = pugi::cast<uint32_t>(multiplierAttribute.value());
				}

				stages.emplace_back(minLevel, maxLevel, multiplier);
			}
		}

                std::sort(stages.begin(), stages.end());
                return stages;
        }

        std::vector<ConfigManager::MonsterLevelRule> loadMonsterLevelRules(lua_State* L) {
                std::vector<ConfigManager::MonsterLevelRule> rules;

                lua_getglobal(L, "monsterLevelRules");
                if (!lua_istable(L, -1)) {
                        lua_pop(L, 1);
                        return rules;
                }

                lua_pushnil(L);
                while (lua_next(L, -2) != 0) {
                        const auto tableIndex = lua_gettop(L);
                        ConfigManager::MonsterLevelRule rule;
                        rule.minZ = lua::getField<int32_t>(L, tableIndex, "minZ", 0);
                        rule.maxZ = lua::getField<int32_t>(L, tableIndex, "maxZ", static_cast<int32_t>(rule.minZ));
                        rule.levelsPerFloor = lua::getField<int32_t>(L, tableIndex, "levelsPerFloor", 1);
                        rules.push_back(rule);
                        lua_pop(L, 4);
                }
                lua_pop(L, 1);

                std::sort(rules.begin(), rules.end(), [](const auto& a, const auto& b) {
                        return a.minZ < b.minZ;
                });
                return rules;
        }

}

bool ConfigManager::load() {
	lua_State* L = luaL_newstate();
	if (!L) {
		throw std::runtime_error("Failed to allocate memory");
	}

	luaL_openlibs(L);

	string[CONFIG_FILE] = "config.lua";
	if (string[CONFIG_FILE].empty()) {
		string[CONFIG_FILE] = "config.lua";
	}

	if (luaL_dofile(L, string[CONFIG_FILE].data())) {
		std::cout << "[Error - ConfigManager::load] " << lua_tostring(L, -1) << std::endl;
		lua_close(L);
		return false;
	}

	//parse config
	if (!loaded) { //info that must be loaded one time (unless we reset the modules involved)
		boolean[BIND_ONLY_GLOBAL_ADDRESS] = getGlobalBoolean(L, "bindOnlyGlobalAddress", false);
		boolean[OPTIMIZE_DATABASE] = getGlobalBoolean(L, "startupDatabaseOptimization", true);

		if (string[IP] == "") {
			string[IP] = getGlobalString(L, "ip", "127.0.0.1");
		}

		string[MAP_NAME] = getGlobalString(L, "mapName", "forgotten");
		string[MAP_AUTHOR] = getGlobalString(L, "mapAuthor", "Unknown");
		string[HOUSE_RENT_PERIOD] = getGlobalString(L, "houseRentPeriod", "never");
		string[MYSQL_HOST] = getGlobalString(L, "mysqlHost", "127.0.0.1");
		string[MYSQL_USER] = getGlobalString(L, "mysqlUser", "forgottenserver");
		string[MYSQL_PASS] = getGlobalString(L, "mysqlPass", "");
		string[MYSQL_DB] = getGlobalString(L, "mysqlDatabase", "forgottenserver");
		string[MYSQL_SOCK] = getGlobalString(L, "mysqlSock", "");

		integer[SQL_PORT] = getGlobalNumber(L, "mysqlPort", 3306);

		if (integer[GAME_PORT] == 0) {
			integer[GAME_PORT] = getGlobalNumber(L, "gameProtocolPort", 7172);
		}

		if (integer[LOGIN_PORT] == 0) {
			integer[LOGIN_PORT] = getGlobalNumber(L, "loginProtocolPort", 7171);
		}

		integer[STATUS_PORT] = getGlobalNumber(L, "statusProtocolPort", 7171);

		integer[MARKET_OFFER_DURATION] = getGlobalNumber(L, "marketOfferDuration", 30 * 24 * 60 * 60);
	}

	boolean[ALLOW_CHANGEOUTFIT] = getGlobalBoolean(L, "allowChangeOutfit", true);
	boolean[ONE_PLAYER_ON_ACCOUNT] = getGlobalBoolean(L, "onePlayerOnlinePerAccount", true);
	boolean[AIMBOT_HOTKEY_ENABLED] = getGlobalBoolean(L, "hotkeyAimbotEnabled", true);
	boolean[REMOVE_RUNE_CHARGES] = getGlobalBoolean(L, "removeChargesFromRunes", true);
	boolean[REMOVE_WEAPON_AMMO] = getGlobalBoolean(L, "removeWeaponAmmunition", true);
	boolean[REMOVE_WEAPON_CHARGES] = getGlobalBoolean(L, "removeWeaponCharges", true);
	boolean[REMOVE_POTION_CHARGES] = getGlobalBoolean(L, "removeChargesFromPotions", true);
	boolean[EXPERIENCE_FROM_PLAYERS] = getGlobalBoolean(L, "experienceByKillingPlayers", false);
	boolean[FREE_PREMIUM] = getGlobalBoolean(L, "freePremium", false);
	boolean[REPLACE_KICK_ON_LOGIN] = getGlobalBoolean(L, "replaceKickOnLogin", true);
	boolean[ALLOW_CLONES] = getGlobalBoolean(L, "allowClones", false);
	boolean[ALLOW_WALKTHROUGH] = getGlobalBoolean(L, "allowWalkthrough", true);
	boolean[MARKET_PREMIUM] = getGlobalBoolean(L, "premiumToCreateMarketOffer", true);
	boolean[EMOTE_SPELLS] = getGlobalBoolean(L, "emoteSpells", false);
	boolean[STAMINA_SYSTEM] = getGlobalBoolean(L, "staminaSystem", true);
	boolean[WARN_UNSAFE_SCRIPTS] = getGlobalBoolean(L, "warnUnsafeScripts", true);
	boolean[CONVERT_UNSAFE_SCRIPTS] = getGlobalBoolean(L, "convertUnsafeScripts", true);
	boolean[CLASSIC_EQUIPMENT_SLOTS] = getGlobalBoolean(L, "classicEquipmentSlots", false);
	boolean[CLASSIC_ATTACK_SPEED] = getGlobalBoolean(L, "classicAttackSpeed", false);
	boolean[SCRIPTS_CONSOLE_LOGS] = getGlobalBoolean(L, "showScriptsLogInConsole", true);
	boolean[SERVER_SAVE_NOTIFY_MESSAGE] = getGlobalBoolean(L, "serverSaveNotifyMessage", true);
	boolean[SERVER_SAVE_CLEAN_MAP] = getGlobalBoolean(L, "serverSaveCleanMap", false);
	boolean[SERVER_SAVE_CLOSE] = getGlobalBoolean(L, "serverSaveClose", false);
	boolean[SERVER_SAVE_SHUTDOWN] = getGlobalBoolean(L, "serverSaveShutdown", true);
	boolean[ONLINE_OFFLINE_CHARLIST] = getGlobalBoolean(L, "showOnlineStatusInCharlist", false);
	boolean[YELL_ALLOW_PREMIUM] = getGlobalBoolean(L, "yellAlwaysAllowPremium", false);
	boolean[PREMIUM_TO_SEND_PRIVATE] = getGlobalBoolean(L, "premiumToSendPrivate", false);
	boolean[FORCE_MONSTERTYPE_LOAD] = getGlobalBoolean(L, "forceMonsterTypesOnLoad", true);
	boolean[DEFAULT_WORLD_LIGHT] = getGlobalBoolean(L, "defaultWorldLight", true);
	boolean[HOUSE_OWNED_BY_ACCOUNT] = getGlobalBoolean(L, "houseOwnedByAccount", false);
	boolean[CLEAN_PROTECTION_ZONES] = getGlobalBoolean(L, "cleanProtectionZones", false);
	boolean[HOUSE_DOOR_SHOW_PRICE] = getGlobalBoolean(L, "houseDoorShowPrice", true);
	boolean[ONLY_INVITED_CAN_MOVE_HOUSE_ITEMS] = getGlobalBoolean(L, "onlyInvitedCanMoveHouseItems", true);
	boolean[REMOVE_ON_DESPAWN] = getGlobalBoolean(L, "removeOnDespawn", true);
	boolean[PLAYER_CONSOLE_LOGS] = getGlobalBoolean(L, "showPlayerLogInConsole", true);
        boolean[CHECK_DUPLICATE_STORAGE_KEYS] = getGlobalBoolean(L, "checkDuplicateStorageKeys", false);
        boolean[MONSTER_OVERSPAWN] = getGlobalBoolean(L, "monsterOverspawn", false);
        boolean[MONSTER_LEVEL_SCALING] = getGlobalBoolean(L, "enableMonsterLevelScaling", false);

	string[DEFAULT_PRIORITY] = getGlobalString(L, "defaultPriority", "high");
	string[SERVER_NAME] = getGlobalString(L, "serverName", "");
	string[OWNER_NAME] = getGlobalString(L, "ownerName", "");
	string[OWNER_EMAIL] = getGlobalString(L, "ownerEmail", "");
	string[URL] = getGlobalString(L, "url", "");
	string[LOCATION] = getGlobalString(L, "location", "");
	string[MOTD] = getGlobalString(L, "motd", "");
        string[WORLD_TYPE] = getGlobalString(L, "worldType", "pvp");

        floats[MONSTER_BONUS_HEALTH] = getGlobalFloat(L, "monsterBonusHealth", 0.f);
        floats[MONSTER_BONUS_DAMAGE] = getGlobalFloat(L, "monsterBonusDamage", 0.f);
        floats[MONSTER_BONUS_SPEED] = getGlobalFloat(L, "monsterBonusSpeed", 0.f);
        floats[MONSTER_BONUS_LOOT]   = getGlobalFloat(L, "monsterBonusLoot", 0.f);

        monsterLevelRules = loadMonsterLevelRules(L);

	integer[MAX_PLAYERS] = getGlobalNumber(L, "maxPlayers");
	integer[PZ_LOCKED] = getGlobalNumber(L, "pzLocked", 60000);
	integer[DEFAULT_DESPAWNRANGE] = Monster::despawnRange = getGlobalNumber(L, "deSpawnRange", 2);
	integer[DEFAULT_DESPAWNRADIUS] = Monster::despawnRadius = getGlobalNumber(L, "deSpawnRadius", 50);
	integer[DEFAULT_WALKTOSPAWNRADIUS] = getGlobalNumber(L, "walkToSpawnRadius", 15);
	integer[RATE_EXPERIENCE] = getGlobalNumber(L, "rateExp", 5);
	integer[RATE_SKILL] = getGlobalNumber(L, "rateSkill", 3);
	integer[RATE_LOOT] = getGlobalNumber(L, "rateLoot", 2);
	integer[RATE_MAGIC] = getGlobalNumber(L, "rateMagic", 3);
	integer[RATE_SPAWN] = getGlobalNumber(L, "rateSpawn", 1);
	integer[HOUSE_PRICE] = getGlobalNumber(L, "housePriceEachSQM", 1000);
	integer[KILLS_TO_RED] = getGlobalNumber(L, "killsToRedSkull", 3);
	integer[KILLS_TO_BLACK] = getGlobalNumber(L, "killsToBlackSkull", 6);
	integer[ACTIONS_DELAY_INTERVAL] = getGlobalNumber(L, "timeBetweenActions", 200);
	integer[EX_ACTIONS_DELAY_INTERVAL] = getGlobalNumber(L, "timeBetweenExActions", 1000);
	integer[MAX_MESSAGEBUFFER] = getGlobalNumber(L, "maxMessageBuffer", 4);
	integer[KICK_AFTER_MINUTES] = getGlobalNumber(L, "kickIdlePlayerAfterMinutes", 15);
	integer[PROTECTION_LEVEL] = getGlobalNumber(L, "protectionLevel", 1);
	integer[DEATH_LOSE_PERCENT] = getGlobalNumber(L, "deathLosePercent", -1);
	integer[STATUSQUERY_TIMEOUT] = getGlobalNumber(L, "statusTimeout", 5000);
	integer[FRAG_TIME] = getGlobalNumber(L, "timeToDecreaseFrags", 24 * 60 * 60);
	integer[WHITE_SKULL_TIME] = getGlobalNumber(L, "whiteSkullTime", 15 * 60);
	integer[STAIRHOP_DELAY] = getGlobalNumber(L, "stairJumpExhaustion", 2000);
	integer[EXP_FROM_PLAYERS_LEVEL_RANGE] = getGlobalNumber(L, "expFromPlayersLevelRange", 75);
	integer[CHECK_EXPIRED_MARKET_OFFERS_EACH_MINUTES] = getGlobalNumber(L, "checkExpiredMarketOffersEachMinutes", 60);
	integer[MAX_MARKET_OFFERS_AT_A_TIME_PER_PLAYER] = getGlobalNumber(L, "maxMarketOffersAtATimePerPlayer", 100);
	integer[MAX_PACKETS_PER_SECOND] = getGlobalNumber(L, "maxPacketsPerSecond", 25);
	integer[SERVER_SAVE_NOTIFY_DURATION] = getGlobalNumber(L, "serverSaveNotifyDuration", 5);
	integer[YELL_MINIMUM_LEVEL] = getGlobalNumber(L, "yellMinimumLevel", 2);
	integer[MINIMUM_LEVEL_TO_SEND_PRIVATE] = getGlobalNumber(L, "minimumLevelToSendPrivate", 1);
	integer[VIP_FREE_LIMIT] = getGlobalNumber(L, "vipFreeLimit", 20);
	integer[VIP_PREMIUM_LIMIT] = getGlobalNumber(L, "vipPremiumLimit", 100);
	integer[DEPOT_FREE_LIMIT] = getGlobalNumber(L, "depotFreeLimit", 2000);
	integer[DEPOT_PREMIUM_LIMIT] = getGlobalNumber(L, "depotPremiumLimit", 10000);
	integer[STAMINA_REGEN_MINUTE] = getGlobalNumber(L, "timeToRegenMinuteStamina", 3 * 60);
	integer[STAMINA_REGEN_PREMIUM] = getGlobalNumber(L, "timeToRegenMinutePremiumStamina", 10 * 60);
	integer[PATHFINDING_INTERVAL] = getGlobalNumber(L, "pathfindingInterval", 200);
	integer[PATHFINDING_DELAY] = getGlobalNumber(L, "pathfindingDelay", 300);

	expStages = loadXMLStages();
	if (expStages.empty()) {
		expStages = loadLuaStages(L);
	} else {
		std::cout << "[Warning - ConfigManager::load] XML stages are deprecated, consider moving to config.lua." << std::endl;
	}
	expStages.shrink_to_fit();

	loaded = true;
	lua_close(L);

	return true;
}

bool ConfigManager::reload() {
	bool result = load();
	if (transformToSHA1(getString(ConfigManager::MOTD)) != g_game.getMotdHash()) {
		g_game.incrementMotdNum();
	}
	return result;
}

const std::string& ConfigManager::getString(string_config_t what) {
	static std::string dummyStr;

	if (what >= LAST_STRING_CONFIG) {
		std::cout << "[Warning - ConfigManager::getString] Accessing invalid index: " << what << std::endl;
		return dummyStr;
	}
	return string[what];
}

int32_t ConfigManager::getNumber(integer_config_t what) {
	if (what >= LAST_INTEGER_CONFIG) {
		std::cout << "[Warning - ConfigManager::getNumber] Accessing invalid index: " << what << std::endl;
		return 0;
	}
	return integer[what];
}

bool ConfigManager::getBoolean(boolean_config_t what) {
        if (what >= LAST_BOOLEAN_CONFIG) {
                std::cout << "[Warning - ConfigManager::getBoolean] Accessing invalid index: " << what << std::endl;
                return false;
        }
        return boolean[what];
}

float ConfigManager::getFloat(float_config_t what) {
        if (what >= LAST_FLOAT_CONFIG) {
                std::cout << "[Warning - ConfigManager::getFloat] Accessing invalid index: " << what << std::endl;
                return 0.f;
        }
        return floats[what];
}

float ConfigManager::getExperienceStage(uint32_t level) {
	auto it = std::find_if(expStages.begin(), expStages.end(), [level](auto&& stage) {
		auto&& [minLevel, maxLevel, _] = stage;
		return level >= minLevel && level <= maxLevel;
	});

	if (it == expStages.end()) {
		return getNumber(ConfigManager::RATE_EXPERIENCE);
	}

        return std::get<2>(*it);
}

const std::vector<ConfigManager::MonsterLevelRule>& ConfigManager::getMonsterLevelRules() {
        return monsterLevelRules;
}

bool ConfigManager::setString(string_config_t what, std::string_view value) {
	if (what >= LAST_STRING_CONFIG) {
		std::cout << "[Warning - ConfigManager::setString] Accessing invalid index: " << what << std::endl;
		return false;
	}

	string[what] = value;
	return true;
}

bool ConfigManager::setNumber(integer_config_t what, int32_t value) {
	if (what >= LAST_INTEGER_CONFIG) {
		std::cout << "[Warning - ConfigManager::setNumber] Accessing invalid index: " << what << std::endl;
		return false;
	}

	integer[what] = value;
	return true;
}

bool ConfigManager::setBoolean(boolean_config_t what, bool value) {
	if (what >= LAST_BOOLEAN_CONFIG) {
		std::cout << "[Warning - ConfigManager::setBoolean] Accessing invalid index: " << what << std::endl;
		return false;
	}

	boolean[what] = value;
	return true;
}