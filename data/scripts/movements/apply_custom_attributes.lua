function onEquipCustomAttributes(player, item, slot, isCheck)
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

function onDeEquipCustomAttributes(player, item, slot, isCheck)
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
