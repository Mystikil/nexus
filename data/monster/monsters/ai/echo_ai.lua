
function onThink(monster)
    if not monster then return end
    if not monster:getTarget() then return true end

    local memory = monster:getStorageValue(10000)
    if memory == -1 then memory = 0 end
    memory = memory + 1

    -- Adaptive behavior: flee if low HP and remembering last danger level
    if monster:getHealth() < (monster:getMaxHealth() * 0.3) and memory > 5 then
        monster:say("Strategic retreat initiated...", TALKTYPE_MONSTER_SAY)
        monster:teleportTo(monster:getPosition():getNextPosition(1))
    end

    monster:setStorageValue(10000, memory)
    return true
end
