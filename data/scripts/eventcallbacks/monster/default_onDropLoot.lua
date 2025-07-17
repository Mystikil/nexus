local event = Event()

event.onDropLoot = function(self, corpse)
	if configManager.getNumber(configKeys.RATE_LOOT) == 0 then
		return
	end

	local player = Player(corpse:getCorpseOwner())
	local mType = self:getType()
	local doCreateLoot = false

	if not player or player:getStamina() > 840 or not configManager.getBoolean(configKeys.STAMINA_SYSTEM) then
		doCreateLoot = true
	end

        if doCreateLoot then
                local monsterLoot = mType:getLoot()
                local bonus = 0
                if player then
                        bonus = player:getCustomAttribute(20)
                end
                for i = 1, #monsterLoot do
                        local entry = monsterLoot[i]
                        local chance = entry.chance
                        if bonus > 0 then
                                chance = math.min(100000, chance + math.floor(chance * bonus / 100))
                        end
                         local item = corpse:createLootItem({itemId = entry.itemId, maxCount = entry.maxCount, chance = chance})
                        if not item then
                                print("[Warning] DropLoot: Could not add loot item to corpse.")
                        end
                end
        end

	if player then
		local text
		if doCreateLoot then
			text = ("Loot of %s: %s."):format(mType:getNameDescription(), corpse:getContentDescription())
		else
			text = ("Loot of %s: nothing (due to low stamina)."):format(mType:getNameDescription())
		end
		local party = player:getParty()
		if party then
			party:broadcastPartyLoot(text)
		else
			player:sendTextMessage(MESSAGE_LOOT, text)
		end
	end
end

event:register()
