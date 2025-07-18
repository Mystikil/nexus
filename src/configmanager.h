// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_CONFIGMANAGER_H
#define FS_CONFIGMANAGER_H

namespace ConfigManager {

        enum boolean_config_t {
		ALLOW_CHANGEOUTFIT,
		ONE_PLAYER_ON_ACCOUNT,
		AIMBOT_HOTKEY_ENABLED,
		REMOVE_RUNE_CHARGES,
		REMOVE_WEAPON_AMMO,
		REMOVE_WEAPON_CHARGES,
		REMOVE_POTION_CHARGES,
		EXPERIENCE_FROM_PLAYERS,
		FREE_PREMIUM,
		REPLACE_KICK_ON_LOGIN,
		ALLOW_CLONES,
		ALLOW_WALKTHROUGH,
		BIND_ONLY_GLOBAL_ADDRESS,
		OPTIMIZE_DATABASE,
		MARKET_PREMIUM,
		EMOTE_SPELLS,
		STAMINA_SYSTEM,
		WARN_UNSAFE_SCRIPTS,
		CONVERT_UNSAFE_SCRIPTS,
		CLASSIC_EQUIPMENT_SLOTS,
		CLASSIC_ATTACK_SPEED,
		SCRIPTS_CONSOLE_LOGS,
		SERVER_SAVE_NOTIFY_MESSAGE,
		SERVER_SAVE_CLEAN_MAP,
		SERVER_SAVE_CLOSE,
		SERVER_SAVE_SHUTDOWN,
		ONLINE_OFFLINE_CHARLIST,
		YELL_ALLOW_PREMIUM,
		PREMIUM_TO_SEND_PRIVATE,
		FORCE_MONSTERTYPE_LOAD,
		DEFAULT_WORLD_LIGHT,
		HOUSE_OWNED_BY_ACCOUNT,
		CLEAN_PROTECTION_ZONES,
		HOUSE_DOOR_SHOW_PRICE,
		ONLY_INVITED_CAN_MOVE_HOUSE_ITEMS,
		REMOVE_ON_DESPAWN,
		PLAYER_CONSOLE_LOGS,
		CHECK_DUPLICATE_STORAGE_KEYS,
                MONSTER_OVERSPAWN,
                MONSTER_LEVEL_SCALING,

                LAST_BOOLEAN_CONFIG /* this must be the last one */
        };

	enum string_config_t {
		MAP_NAME,
		HOUSE_RENT_PERIOD,
		SERVER_NAME,
		OWNER_NAME,
		OWNER_EMAIL,
		URL,
		LOCATION,
		IP,
		MOTD,
		WORLD_TYPE,
		MYSQL_HOST,
		MYSQL_USER,
		MYSQL_PASS,
		MYSQL_DB,
		MYSQL_SOCK,
		DEFAULT_PRIORITY,
		MAP_AUTHOR,
		CONFIG_FILE,

		LAST_STRING_CONFIG /* this must be the last one */
	};

        enum integer_config_t {
		SQL_PORT,
		MAX_PLAYERS,
		PZ_LOCKED,
		DEFAULT_DESPAWNRANGE,
		DEFAULT_DESPAWNRADIUS,
		DEFAULT_WALKTOSPAWNRADIUS,
		RATE_EXPERIENCE,
		RATE_SKILL,
		RATE_LOOT,
		RATE_MAGIC,
		RATE_SPAWN,
		HOUSE_PRICE,
		KILLS_TO_RED,
		KILLS_TO_BLACK,
		MAX_MESSAGEBUFFER,
		ACTIONS_DELAY_INTERVAL,
		EX_ACTIONS_DELAY_INTERVAL,
		KICK_AFTER_MINUTES,
		PROTECTION_LEVEL,
		DEATH_LOSE_PERCENT,
		STATUSQUERY_TIMEOUT,
		FRAG_TIME,
		WHITE_SKULL_TIME,
		GAME_PORT,
		LOGIN_PORT,
		STATUS_PORT,
		STAIRHOP_DELAY,
		MARKET_OFFER_DURATION,
		CHECK_EXPIRED_MARKET_OFFERS_EACH_MINUTES,
		MAX_MARKET_OFFERS_AT_A_TIME_PER_PLAYER,
		EXP_FROM_PLAYERS_LEVEL_RANGE,
		MAX_PACKETS_PER_SECOND,
		SERVER_SAVE_NOTIFY_DURATION,
		YELL_MINIMUM_LEVEL,
		MINIMUM_LEVEL_TO_SEND_PRIVATE,
		VIP_FREE_LIMIT,
		VIP_PREMIUM_LIMIT,
		DEPOT_FREE_LIMIT,
		DEPOT_PREMIUM_LIMIT,
		STAMINA_REGEN_MINUTE,
		STAMINA_REGEN_PREMIUM,
		PATHFINDING_INTERVAL,
                PATHFINDING_DELAY,

                LAST_INTEGER_CONFIG /* this must be the last one */
        };

        enum float_config_t {
                MONSTER_BONUS_HEALTH,
                MONSTER_BONUS_DAMAGE,
                MONSTER_BONUS_SPEED,
                MONSTER_BONUS_LOOT,

                LAST_FLOAT_CONFIG /* this must be the last one */
        };

        struct MonsterLevelRule {
                int32_t minZ;
                int32_t maxZ;
                int32_t levelsPerFloor;
        };

	bool load();
	bool reload();

        const std::string& getString(string_config_t what);
        int32_t getNumber(integer_config_t what);
        bool getBoolean(boolean_config_t what);
        float getFloat(float_config_t what);
        float getExperienceStage(uint32_t level);
        const std::vector<MonsterLevelRule>& getMonsterLevelRules();

	bool setString(string_config_t what, std::string_view value);
	bool setNumber(integer_config_t what, int32_t value);
	bool setBoolean(boolean_config_t what, bool value);

}; // namespace ConfigManager

#endif // FS_CONFIGMANAGER_H