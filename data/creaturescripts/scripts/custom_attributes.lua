local event = CreatureEvent("CustomAttributes")

function event.onLogin(player)
    player:registerEvent("CustomAttributes")
    local weight = player:getCustomAttribute(18)
    if weight > 0 then
        player:setCapacity(player:getCapacity() + weight * 10)
    end
    if player:getCustomAttribute(28) > 0 then
        player:setLight(215, 7)
    end
    return true
end

function event.onThink(creature, interval)
    if creature:isPlayer() then
        local aggro = creature:getCustomAttribute(22)
        if aggro > 0 then
            local pos = creature:getPosition()
            local mobs = Game.getSpectators(pos, false, false, 5, 5, 5, 5)
            for _, mob in ipairs(mobs) do
                if mob:isMonster() and (not mob:getTarget() or math.random(100) <= aggro) then
                    mob:setTarget(creature)
                end
            end
        end
        local silence = creature:getCustomAttribute(13)
        if silence > 0 and creature:hasCondition(CONDITION_MUTED) and math.random(100) <= silence then
            creature:removeCondition(CONDITION_MUTED)
        end
        local curse = creature:getCustomAttribute(16)
        if curse > 0 and creature:hasCondition(CONDITION_CURSED) and math.random(100) <= curse then
            creature:removeCondition(CONDITION_CURSED)
        end
        local aura = creature:getCustomAttribute(29)
        if aura > 0 then
            local pos = creature:getPosition()
            local spectators = Game.getSpectators(pos, false, true, 1, 1, 1, 1)
            for _, target in ipairs(spectators) do
                if target ~= creature then
                    doTargetCombatHealth(creature, target, COMBAT_FIREDAMAGE, -aura, -aura, CONST_ME_HITBYFIRE)
                end
            end
        end
    end
    return true
end


function event.onHealthChange(creature, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)
    local targetPlayer = creature:getPlayer()
    local attackerPlayer = attacker and attacker:getPlayer()
    if targetPlayer then
        local chance = targetPlayer:getCustomAttribute(2)
        if chance > 0 and math.random(100) <= chance then
            creature:getPosition():sendMagicEffect(CONST_ME_BLOCKHIT)
            return 0, primaryType, 0, secondaryType
        end
        local parry = targetPlayer:getCustomAttribute(3)
        if parry > 0 and math.random(100) <= parry then
            primaryDamage = math.floor(primaryDamage * 0.5)
        end
        local dodge = targetPlayer:getCustomAttribute(4)
        if dodge > 0 and math.random(100) <= dodge then
            creature:getPosition():sendMagicEffect(CONST_ME_POFF)
            return 0, primaryType, 0, secondaryType
        end
        local reflect = targetPlayer:getCustomAttribute(5)
        if reflect > 0 and attacker then
            local refl = math.ceil(-primaryDamage * reflect / 100)
            doTargetCombatHealth(targetPlayer, attacker, COMBAT_PHYSICALDAMAGE, -refl, -refl, CONST_ME_HITAREA)
        end
        local trap = targetPlayer:getCustomAttribute(21)
        if trap > 0 and origin == ORIGIN_NONE then
            primaryDamage = math.floor(primaryDamage * (1 - trap / 100))
        end
        local autoHeal = targetPlayer:getCustomAttribute(24)
        if autoHeal > 0 and math.random(100) <= autoHeal then
            local heal = math.max(5, math.floor(targetPlayer:getLevel() * 0.2))
            targetPlayer:addHealth(heal)
            targetPlayer:getPosition():sendMagicEffect(CONST_ME_MAGIC_BLUE)
        end
        local dur = targetPlayer:getCustomAttribute(25)
        if dur > 0 then
            if creature:hasCondition(CONDITION_POISON) and math.random(100) <= dur then
                creature:removeCondition(CONDITION_POISON)
            end
            if creature:hasCondition(CONDITION_BURN) and math.random(100) <= dur then
                creature:removeCondition(CONDITION_BURN)
            end
            if creature:hasCondition(CONDITION_FREEZING) and math.random(100) <= dur then
                creature:removeCondition(CONDITION_FREEZING)
            end
        end
        local vuln = targetPlayer:getCustomAttribute(14)
        if vuln > 0 and primaryType == COMBAT_HOLYDAMAGE then
            primaryDamage = math.floor(primaryDamage * (1 + vuln / 100))
        end
    end
    if attackerPlayer then
        local ap = attackerPlayer:getCustomAttribute(1)
        if ap > 0 then
            primaryDamage = math.floor(primaryDamage * (1 + ap / 100))
        end
        local trueDmg = attackerPlayer:getCustomAttribute(6)
        if trueDmg > 0 then
            primaryDamage = math.floor(primaryDamage * (1 + trueDmg / 100))
        end
        local bleed = attackerPlayer:getCustomAttribute(7)
        if bleed > 0 and math.random(100) <= bleed then
            local cond = Condition(CONDITION_BLEEDING)
            cond:setParameter(CONDITION_PARAM_DELAYED, true)
            cond:addDamage(5, 2000, -10)
            creature:addCondition(cond)
        end
        local exec = attackerPlayer:getCustomAttribute(8)
        if exec > 0 and creature:getHealth() < creature:getMaxHealth() * 0.2 then
            primaryDamage = math.floor(primaryDamage * (1 + exec / 100))
        end
        local knock = attackerPlayer:getCustomAttribute(9)
        if knock > 0 then
            local pos = creature:getPosition()
            pos:getNextPosition(attacker:getDirection(), knock)
            creature:teleportTo(pos, true)
        end
        local stun = attackerPlayer:getCustomAttribute(10)
        if stun > 0 and math.random(100) <= stun then
            local cond = Condition(CONDITION_PARALYZE)
            cond:setParameter(CONDITION_PARAM_TICKS, 2000)
            cond:setParameter(CONDITION_PARAM_SPEED, -300)
            creature:addCondition(cond)
        end
        if origin == ORIGIN_SPELL then
            local sp = attackerPlayer:getCustomAttribute(11)
            if sp > 0 then
                primaryDamage = math.floor(primaryDamage * (1 + sp / 100))
            end
        end
        local surge = attackerPlayer:getCustomAttribute(15)
        if surge > 0 and math.random(100) <= surge then
            doTargetCombatHealth(attackerPlayer, creature, primaryType, -surge, -surge, CONST_ME_MAGIC_RED)
        end
        local chaos = attackerPlayer:getCustomAttribute(17)
        if chaos > 0 and secondaryType ~= COMBAT_NONE then
            primaryDamage = math.floor(primaryDamage * (1 + chaos / 100))
            secondaryDamage = math.floor(secondaryDamage * (1 + chaos / 100))
        end
    elseif attacker and attacker:getMaster() and attacker:getMaster():isPlayer() then
        local bonus = attacker:getMaster():getCustomAttribute(26)
        if bonus > 0 then
            primaryDamage = math.floor(primaryDamage * (1 + bonus / 100))
        end
    end
    return primaryDamage, primaryType, secondaryDamage, secondaryType
end

function event.onManaChange(creature, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)
    local player = creature:getPlayer()
    if player and primaryDamage < 0 and origin == ORIGIN_SPELL then
        local eff = player:getCustomAttribute(12)
        if eff > 0 then
            primaryDamage = math.floor(primaryDamage * (1 - eff / 100))
        end
    end
    return primaryDamage, primaryType, secondaryDamage, secondaryType
end

event:register()
