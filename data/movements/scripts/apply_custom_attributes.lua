-- Placeholder: Expand with your actual stat system when ready

local function hasCustomAttributes(item)
    -- This checks if the item description contains the keyword "custom"
    return item and item:getAttribute(ITEM_ATTRIBUTE_DESCRIPTION):lower():find("custom")
end

function onEquip(player, item, slot)
    if not hasCustomAttributes(item) then return true end

    player:sendTextMessage(MESSAGE_STATUS_CONSOLE_ORANGE, "[DEBUG] Equipping custom item: " .. item:getName())
    -- Example: Apply extra speed or damage later
    -- player:changeSpeed(10)

    return true
end

function onDeEquip(player, item, slot)
    if not hasCustomAttributes(item) then return true end

    player:sendTextMessage(MESSAGE_STATUS_CONSOLE_ORANGE, "[DEBUG] De-equipping custom item: " .. item:getName())
    -- Example: Revert changes
    -- player:changeSpeed(-10)

    return true
end
