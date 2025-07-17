-- Map Chunk Loader System
MapChunkLoader = MapChunkLoader or {}
MapChunkLoader.instances = MapChunkLoader.instances or {}
MapChunkLoader.nextId = MapChunkLoader.nextId or 1

local function rotatePosition(pos, rotation, size)
    local x, y, z = pos.x or 0, pos.y or 0, pos.z or 0
    if rotation == 90 then
        x, y = size.y - 1 - y, x
    elseif rotation == 180 then
        x, y = size.x - 1 - x, size.y - 1 - y
    elseif rotation == 270 then
        x, y = y, size.x - 1 - x
    end
    return {x = x, y = y, z = z}
end

local function toWorld(pos, base, rotation, size)
    local r = rotatePosition(pos, rotation, size)
    return Position(base.x + r.x, base.y + r.y, base.z + r.z)
end

local function loadData(chunkNameOrTable)
    if type(chunkNameOrTable) == 'table' then
        return chunkNameOrTable, chunkNameOrTable.name or 'chunk'
    end
    local name = chunkNameOrTable
    local path = 'data/chunks/' .. name
    if not path:match('%.lua$') then
        path = path .. '.lua'
    end
    local chunk = dofile(path)
    return chunk, name
end

function MapChunkLoader.loadChunk(chunkSource, basePosition, rotation, options)
    local chunkData, chunkName = loadData(chunkSource)
    rotation = rotation or 0
    options = options or {}
    local owner = options.owner

    local instanceId = MapChunkLoader.nextId
    MapChunkLoader.nextId = instanceId + 1

    local instance = {
        id = instanceId,
        name = chunkName,
        base = Position(basePosition),
        rotation = rotation,
        owner = owner,
        resetAfter = options.resetAfter,
        source = chunkSource,
        objects = {items = {}, monsters = {}}
    }

    local size = chunkData.size or {x = 1, y = 1, z = 1}

    if chunkData.ground then
        for x = 0, size.x - 1 do
            for y = 0, size.y - 1 do
                local pos = toWorld({x = x, y = y, z = 0}, instance.base, rotation, size)
                local item = Game.createItem(chunkData.ground, 1, pos)
                if item then
                    table.insert(instance.objects.items, item)
                end
            end
        end
    end

    if chunkData.items then
        for _, itemInfo in ipairs(chunkData.items) do
            local pos = toWorld(itemInfo.pos, instance.base, rotation, size)
            local item = Game.createItem(itemInfo.id, itemInfo.count or 1, pos)
            if item then
                if itemInfo.actionId then
                    item:setActionId(itemInfo.actionId)
                end
                if itemInfo.container then
                    for _, inside in ipairs(itemInfo.container) do
                        item:addItem(inside.id, inside.count or 1)
                    end
                end
                if itemInfo.effect then
                    pos:sendMagicEffect(itemInfo.effect)
                end
                table.insert(instance.objects.items, item)
            end
        end
    end

    if chunkData.monsters then
        for _, mon in ipairs(chunkData.monsters) do
            local pos = toWorld(mon.pos, instance.base, rotation, size)
            local monster = Game.createMonster(mon.name, pos, true, false, mon.effect or CONST_ME_TELEPORT)
            if monster then
                table.insert(instance.objects.monsters, monster)
            end
        end
    end

    if chunkData.effects then
        for _, eff in ipairs(chunkData.effects) do
            local pos = toWorld(eff.pos, instance.base, rotation, size)
            pos:sendMagicEffect(eff.effect)
        end
    end

    MapChunkLoader.instances[instanceId] = instance
    if owner then
        Game.setStorageValue('chunk_owner:' .. instanceId, owner)
    end

    if instance.resetAfter and instance.resetAfter > 0 then
        instance.resetEvent = addEvent(function()
            MapChunkLoader.unloadChunk(instanceId)
            MapChunkLoader.loadChunk(chunkSource, basePosition, rotation, options)
        end, instance.resetAfter)
    end

    return instanceId
end

function MapChunkLoader.unloadChunk(id)
    local instance = MapChunkLoader.instances[id]
    if not instance then
        return false
    end

    for _, item in ipairs(instance.objects.items) do
        if item and item.remove then
            item:remove()
        end
    end

    for _, monster in ipairs(instance.objects.monsters) do
        if monster and monster:isMonster() then
            monster:remove()
        end
    end

    if instance.resetEvent then
        stopEvent(instance.resetEvent)
    end

    if instance.owner then
        Game.setStorageValue('chunk_owner:' .. id, -1)
    end

    MapChunkLoader.instances[id] = nil
    return true
end

return MapChunkLoader
