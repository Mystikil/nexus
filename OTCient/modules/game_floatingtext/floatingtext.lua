local OPCODE = ExtendedIds.FloatingText

local function onExtendedOpcode(protocol, opcode, buffer)
    local status, data = pcall(function() return json.decode(buffer) end)
    if not status or not data then
        return
    end

    local creature = g_map.getCreatureById(data.cid)
    if not creature then
        return
    end

    local pos = creature:getPosition()
    local text = tostring(data.amount)
    if data.heal then
        text = '+' .. text
    else
        text = '-' .. text
    end

    g_map.addAnimatedText(AnimatedText.create(text, data.color), pos)
end

function init()
    ProtocolGame.registerExtendedOpcode(OPCODE, onExtendedOpcode)
end

function terminate()
    ProtocolGame.unregisterExtendedOpcode(OPCODE)
end
