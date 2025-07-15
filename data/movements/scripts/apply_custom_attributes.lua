--
-- Movement callback used by <movevent> entries in movements.xml.
-- Adjusts the player's custom attributes when equipping or unequipping
-- items that define them.
--

-- Handles equipping items with custom attributes
function onEquip(player, item, slot, isCheck)
    if isCheck then
        return true
    end

    for id, _ in pairs(CustomAttributes.attributes) do
        local value = item:getCustomAttribute(id)
        if value then
            local current = player:getCustomAttribute(id)
            player:setCustomAttribute(id, current + value)
        end
    end
    return true
end

-- Handles unequipping items with custom attributes
function onDeEquip(player, item, slot, isCheck)
    if isCheck then
        return true
    end

    for id, _ in pairs(CustomAttributes.attributes) do
        local value = item:getCustomAttribute(id)
        if value then
            local current = player:getCustomAttribute(id)
            player:setCustomAttribute(id, current - value)
        end
    end
    return true
end

-- Aliases for compatibility
onEquipCustomAttributes = onEquip
onDeEquipCustomAttributes = onDeEquip
