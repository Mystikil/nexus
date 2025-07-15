-- NPC Reputation System
Reputation = Reputation or {}

local configFile = 'data/npc/reputation_config.xml'

-- default values
Reputation.config = { gain = 1, discount = 10, gift = 20, quest = 30 }
Reputation.storageBase = 200000

local function loadValue(pattern, text, default)
    local v = text:match(pattern)
    if v then
        return tonumber(v)
    end
    return default
end

function Reputation.load()
    local f = io.open(configFile, 'r')
    if not f then
        print('[Warning] reputation_config.xml not found, using defaults.')
        return
    end
    local data = f:read('*a')
    f:close()
    Reputation.config.gain = loadValue('gain="(%%d+)"', data, Reputation.config.gain)
    Reputation.config.discount = loadValue('name="discount"%s+value="(%%d+)"', data, Reputation.config.discount)
    Reputation.config.gift = loadValue('name="gift"%s+value="(%%d+)"', data, Reputation.config.gift)
    Reputation.config.quest = loadValue('name="quest"%s+value="(%%d+)"', data, Reputation.config.quest)
end

local function hash(name)
    local h = 0
    for i = 1, #name do
        h = h + name:byte(i)
    end
    return h
end

function Reputation.getKey(name)
    return Reputation.storageBase + hash(name)
end

function Reputation.add(player, name)
    local key = Reputation.getKey(name)
    local rep = player:getStorageValue(key)
    if rep < 0 then rep = 0 end
    rep = rep + Reputation.config.gain
    player:setStorageValue(key, rep)
    return rep
end

function Reputation.get(player, name)
    local key = Reputation.getKey(name)
    local rep = player:getStorageValue(key)
    if rep < 0 then return 0 end
    return rep
end

function Reputation.priceModifier(player, name)
    local rep = Reputation.get(player, name)
    if rep >= Reputation.config.discount then
        return 0.8 -- 20% discount
    end
    return 1
end

function Reputation.handleGreet(player, name)
    local rep = Reputation.add(player, name)
    local key = Reputation.getKey(name)
    if rep >= Reputation.config.gift and player:getStorageValue(key + 1) ~= 1 then
        player:addItem(2152, 10) -- 10 platinum coins
        player:setStorageValue(key + 1, 1)
        Npc():say('You have helped me a lot, take this small gift.', player)
    elseif rep >= Reputation.config.quest and player:getStorageValue(key + 2) ~= 1 then
        Npc():say('I have a rare quest for you when you are ready.', player)
        player:setStorageValue(key + 2, 1)
    elseif rep >= Reputation.config.discount then
        Npc():say('My prices are cheaper for you now.', player)
    end
end

Reputation.load()
return Reputation
