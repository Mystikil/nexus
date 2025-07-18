local OPCODE = ExtendedIds.Shader or 9

local function onShaderOpcode(protocol, opcode, buffer)
    local status, data = pcall(function() return json.decode(buffer) end)
    if not status or type(data) ~= 'table' then
        return
    end

    local creature
    if data.cid then
        creature = g_map.getCreatureById(data.cid)
    else
        creature = g_game.getLocalPlayer()
    end

    if not creature or not data.shader then
        return
    end

    if data.enabled == false then
        creature:setShader('Default')
        return
    end

    if g_shaders.getShader(data.shader) then
        creature:setShader(data.shader)
    end
end

function init()
    ProtocolGame.registerExtendedOpcode(OPCODE, onShaderOpcode)
end

function terminate()
    ProtocolGame.unregisterExtendedOpcode(OPCODE)
end
