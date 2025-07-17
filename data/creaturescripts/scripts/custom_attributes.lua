function onLogin(player)
	player:registerEvent("CustomAttributesThink")
	player:registerEvent("CustomAttributesHealth")
	player:registerEvent("CustomAttributesMana")

	local weight = player:getCustomAttribute(18)
	if weight > 0 then
		player:setCapacity(player:getCapacity() + weight * 10)
	end

	if player:getCustomAttribute(28) > 0 then
		player:setLight(215, 7)
	end

	return true
end

function onThink(creature, interval)
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

function onHealthChange(creature, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)
	-- unchanged logic here (paste your original block)
	return primaryDamage, primaryType, secondaryDamage, secondaryType
end

function onManaChange(creature, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)
	local player = creature:getPlayer()
	if player and primaryDamage < 0 and origin == ORIGIN_SPELL then
		local eff = player:getCustomAttribute(12)
		if eff > 0 then
			primaryDamage = math.floor(primaryDamage * (1 - eff / 100))
		end
	end
	return primaryDamage, primaryType, secondaryDamage, secondaryType
end
