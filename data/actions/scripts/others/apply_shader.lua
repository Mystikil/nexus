local opcode = OPCODE_SHADER or 9

local function sendShader(target, shaderKey, enabled)
    local shaderInfo = SHADER_LIST[shaderKey]
    if not shaderInfo then
        return false
    end

    local payload = json.encode({ shader = shaderInfo.shader, enabled = enabled })
    target:sendExtendedOpcode(opcode, payload)
    return true
end

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
    local entry = SHADER_ITEMS[item:getId()]
    if not entry then
        return false
    end

    if not target or not target:isPlayer() then
        player:sendCancelMessage('Use this item on a player.')
        return true
    end

    local tgt = Player(target)
    local shaderInfo = SHADER_LIST[entry.key]
    if not shaderInfo then
        return false
    end

    local storage = shaderInfo.storage
    local enabled = true
    if entry.toggle and storage then
        if tgt:getStorageValue(storage) == 1 then
            enabled = false
            tgt:setStorageValue(storage, 0)
        else
            tgt:setStorageValue(storage, 1)
        end
    elseif entry.toggle == false then
        enabled = true
    end

    sendShader(tgt, entry.key, enabled)

    if entry.consume ~= false then
        item:remove(1)
    end
    return true
end
