local creatureevent = CreatureEvent("ECHOThink")
local echo_memory = {}

-- EXP Phase logic
local function getPhaseFromEXP(exp)
    if exp <= 1500 then
        return 1
    elseif exp <= 2500 then
        return 2
    elseif exp <= 3500 then
        return 3
    elseif exp <= 4500 then
        return 4
    else
        return math.floor((exp - 3500) / 1000) + 5
    end
end

-- Safe effect and condition
local function safeEffect(target, effect, cond)
    if target and target:isPlayer() then
        local pos = target:getPosition()
        if cond then target:addCondition(cond) end
        if effect then pos:sendMagicEffect(effect) end
    end
end

function creatureevent.onThink(monster, interval)
    if not monster or not monster:isMonster() then return true end

    local id = monster:getId()
    echo_memory[id] = echo_memory[id] or {
        lastPhase = -1,
        evolved = false,
        lastAction = 0
    }

    local mem = echo_memory[id]
    local now = os.mtime()
    if now - mem.lastAction < 8000 then return true end -- throttle

    mem.lastAction = now
    local pos = monster:getPosition()
    local players = Game.getSpectators(pos, false, true, 7, 7, 5, 5)
    if #players == 0 then return true end

    local target = monster:getTarget() or players[math.random(#players)]
    if target then monster:selectTarget(target) end

    local exp = monster:getType():getExperience()
    if not exp then return true end -- safety fallback

    local phase = getPhaseFromEXP(exp)

    -- Don't repeat same phase
    if phase <= mem.lastPhase then return true end
    mem.lastPhase = phase

    -- Phased behavior
    if phase == 1 then
        local cond = Condition(CONDITION_POISON)
        cond:setParameter(CONDITION_PARAM_TICKS, 4000)
        cond:setParameter(CONDITION_PARAM_DAMAGE, 30)
        for _, p in ipairs(players) do safeEffect(p, CONST_ME_POISONAREA, cond) end

    elseif phase == 2 then
        local cond = Condition(CONDITION_CURSED)
        cond:setParameter(CONDITION_PARAM_TICKS, 4000)
        for _, p in ipairs(players) do safeEffect(p, CONST_ME_MORTAREA, cond) end

    elseif phase == 3 then
        for _, p in ipairs(players) do
            p:addHealth(-math.random(40, 80))
            p:getPosition():sendMagicEffect(CONST_ME_DRAWBLOOD)
        end

    elseif phase == 4 then
        local cond = Condition(CONDITION_PARALYZE)
        cond:setParameter(CONDITION_PARAM_TICKS, 3000)
        cond:setParameter(CONDITION_PARAM_SPEED, -600)
        for _, p in ipairs(players) do safeEffect(p, CONST_ME_STUN, cond) end

    elseif phase == 5 then
        if math.random(100) < 15 then
            Game.createMonster(monster:getName(), pos, true, true)
            pos:sendMagicEffect(CONST_ME_TELEPORT)
        end

    elseif phase == 6 then
        monster:addHealth(math.random(100, 150))
        pos:sendMagicEffect(CONST_ME_MAGIC_GREEN)

    elseif phase == 7 then
        local cond = Condition(CONDITION_BURNING)
        cond:setParameter(CONDITION_PARAM_TICKS, 4000)
        cond:setParameter(CONDITION_PARAM_DAMAGE, 50)
        for _, p in ipairs(players) do safeEffect(p, CONST_ME_FIREAREA, cond) end

    elseif phase == 8 then
        local cond = Condition(CONDITION_ATTRIBUTES)
        cond:setParameter(CONDITION_PARAM_TICKS, 6000)
        cond:setParameter(CONDITION_PARAM_SKILL_MELEEPERCENT, 120)
        cond:setParameter(CONDITION_PARAM_SKILL_MAGICPERCENT, 120)
        cond:setParameter(CONDITION_PARAM_BUFF_SPELL, true)
        cond:setParameter(CONDITION_PARAM_FORCEUPDATE, true)
        monster:addCondition(cond)
        pos:sendMagicEffect(CONST_ME_ENERGYHIT)

    elseif phase == 9 then
        if target then
            target:addHealth(-math.random(100, 200))
            target:getPosition():sendMagicEffect(CONST_ME_EXPLOSIONHIT)
        end

    elseif phase == 10 then
        local cond = Condition(CONDITION_DROWN)
        cond:setParameter(CONDITION_PARAM_TICKS, 4000)
        cond:setParameter(CONDITION_PARAM_DAMAGE, 40)
        for _, p in ipairs(players) do safeEffect(p, CONST_ME_WATERSPLASH, cond) end
    end

    return true
end

creatureevent:register()
