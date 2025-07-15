--
-- Movement callback used by <movevent> entries in movements.xml.
-- Adjusts the player's custom attributes when equipping or unequipping
-- items that define them.
--

-- legacy functions referenced by movements.xml
function onEquip(player, item, slot, isCheck)
    if isCheck then
        return true
    end

    CustomAttributes.recalculatePlayer(player)
    return true
end

function onDeEquip(player, item, slot, isCheck)
    if isCheck then
        return true
    end

    CustomAttributes.recalculatePlayer(player)
    return true
end

-- keep the original function names for compatibility
onEquipCustomAttributes = onEquip
onDeEquipCustomAttributes = onDeEquip
