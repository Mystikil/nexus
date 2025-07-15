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

    CustomAttributes.recalculatePlayer(player)
    return true
end

-- Handles unequipping items with custom attributes
function onDeEquip(player, item, slot, isCheck)
    if isCheck then
        return true
    end

    CustomAttributes.recalculatePlayer(player)
    return true
end

-- Aliases for compatibility
onEquipCustomAttributes = onEquip
onDeEquipCustomAttributes = onDeEquip
