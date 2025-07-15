CustomAttributes = {}
CustomAttributes.attributes = {}
CustomAttributes.storageBase = 220000

function CustomAttributes.load()
	local path = 'data/XML/custom_attributes.xml'
	for line in io.lines(path) do
		local id = tonumber(line:match('id="(%d+)"'))
		if id then
			local name = line:match('name="([^"]+)"') or ''
			local desc = line:match('description="([^"]+)"') or ''
			local group = line:match('group="([^"]+)"') or ''
			CustomAttributes.attributes[id] = {
				name = name,
				description = desc,
				group = group
			}
		end
	end
end

function CustomAttributes.getName(id)
	local info = CustomAttributes.attributes[id]
	return info and info.name or 'Unknown'
end

function CustomAttributes.getDescription(id)
	local info = CustomAttributes.attributes[id]
	return info and info.description or ''
end

function CustomAttributes.getGroup(id)
	local info = CustomAttributes.attributes[id]
	return info and info.group or ''
end

function CustomAttributes.getKey(id)
	return CustomAttributes.storageBase + id
end

function Player.getCustomAttribute(self, id)
	local value = self:getStorageValue(CustomAttributes.getKey(id))
	if value < 0 then
		return 0
	end
	return value
end

function Player.setCustomAttribute(self, id, value)
        self:setStorageValue(CustomAttributes.getKey(id), value)
end

function CustomAttributes.recalculatePlayer(player)
        for id, _ in pairs(CustomAttributes.attributes) do
                player:setCustomAttribute(id, 0)
        end

        local slots = {
                CONST_SLOT_HEAD,
                CONST_SLOT_NECKLACE,
                CONST_SLOT_BACKPACK,
                CONST_SLOT_ARMOR,
                CONST_SLOT_RIGHT,
                CONST_SLOT_LEFT,
                CONST_SLOT_LEGS,
                CONST_SLOT_FEET,
                CONST_SLOT_RING,
                CONST_SLOT_AMMO,
        }

        for _, slot in ipairs(slots) do
                local item = player:getSlotItem(slot)
                if item then
                        for id, _ in pairs(CustomAttributes.attributes) do
                                local value = item:getCustomAttribute(id)
                                if value then
                                        local current = player:getCustomAttribute(id)
                                        player:setCustomAttribute(id, current + value)
                                end
                        end
                end
        end

        local weight = player:getCustomAttribute(18)
        player:setCapacity(player:getCapacity() + weight * 10)

        if player:getCustomAttribute(28) > 0 then
                player:setLight(215, 7)
        else
                player:setLight(0, 0)
        end
end
