-- Shared adaptive AI for echo monsters
-- Stores persistent data in JSON files per monster name.
local EchoAI = {}

-- storage keys
EchoAI.STORAGE_DEATHS = 8000
EchoAI.STORAGE_LAST_HIT = 8001
EchoAI.STORAGE_STATE = 8002
EchoAI.memoryDir = 'data/ai_memory'

local function getFile(monster)
    local name = monster:getName():lower():gsub('%s+', '_')
    return string.format('%s/%s.json', EchoAI.memoryDir, name)
end

local function loadMemory(monster)
    local f = io.open(getFile(monster), 'r')
    if f then
        local t = json.decode(f:read('*a')) or {}
        f:close()
        monster.memory = t
    else
        monster.memory = {}
    end
    monster.memory.deaths = monster.memory.deaths or 0
    monster.memory.lastKillType = monster.memory.lastKillType or -1
end

local function saveMemory(monster)
    local f = io.open(getFile(monster), 'w')
    if f then
        f:write(json.encode(monster.memory))
        f:close()
    end
end

function EchoAI.onSpawn(monster, position, startup, artificial)
    loadMemory(monster)
    monster:setStorageValue(EchoAI.STORAGE_DEATHS, monster.memory.deaths)
    monster:setStorageValue(EchoAI.STORAGE_LAST_HIT, monster.memory.lastKillType)
    monster:setStorageValue(EchoAI.STORAGE_STATE, 0)
    return true
end

function EchoAI.onThink(monster, interval)
    local lastKill = monster.memory.lastKillType
    local state = monster:getStorageValue(EchoAI.STORAGE_STATE)

    if lastKill == COMBAT_FIREDAMAGE and state ~= 1 then
        monster:changeSpeed(-20)
        monster:say('Fire will not burn me again!', TALKTYPE_MONSTER_SAY)
        monster:setStorageValue(EchoAI.STORAGE_STATE, 1)
    elseif lastKill == COMBAT_ICEDAMAGE and state ~= 2 then
        monster:changeSpeed(20)
        monster:say('I adapted to the cold.', TALKTYPE_MONSTER_SAY)
        monster:setStorageValue(EchoAI.STORAGE_STATE, 2)
    end

    if monster.memory.deaths >= 3 and monster:getStorageValue(EchoAI.STORAGE_STATE) ~= 3 then
        monster:say('I return stronger!', TALKTYPE_MONSTER_YELL)
        monster:changeSpeed(30)
        monster:setStorageValue(EchoAI.STORAGE_STATE, 3)
    end
    return true
end

function EchoAI.onHealthChange(monster, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)
    if attacker then
        monster.memory.lastAttackerId = attacker:getId()
    end
    monster:setStorageValue(EchoAI.STORAGE_LAST_HIT, primaryType)
    return primaryDamage, primaryType, secondaryDamage, secondaryType
end

function EchoAI.onDeath(monster, corpse, killer, mostDamage, lastHitUnjustified, mostDamageUnjustified)
    monster.memory.deaths = monster.memory.deaths + 1
    monster.memory.lastKillType = monster:getStorageValue(EchoAI.STORAGE_LAST_HIT)
    saveMemory(monster)
    return true
end

return EchoAI
