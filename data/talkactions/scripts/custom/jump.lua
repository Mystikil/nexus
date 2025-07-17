function onSay(player, words, param)
    local dist = player:getCustomAttribute(23)
    if dist <= 0 then
        player:sendCancelMessage("You cannot jump.")
        return false
    end
    local pos = player:getPosition()
    pos:getNextPosition(player:getDirection(), dist)
    player:teleportTo(pos, true)
    pos:sendMagicEffect(CONST_ME_TELEPORT)
    return false
end
