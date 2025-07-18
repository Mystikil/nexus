local persistence = {}

local useDb = db and type(db.query) == 'function'
local basePath = 'data/monster/memory/'

local function ensureDir()
    if not useDb then
        os.execute('mkdir -p '..basePath)
    end
end

function persistence.saveMemory(key, data)
    ensureDir()
    if useDb then
        if data == nil then
            db.query("DELETE FROM `monster_memory` WHERE `mem_key` = " .. db.escapeString(key))
            return
        end
        local encoded = json.encode(data)
        db.query(string.format("REPLACE INTO `monster_memory` (`mem_key`, `data`) VALUES (%s, %s)", db.escapeString(key), db.escapeString(encoded)))
    else
        local path = basePath .. key .. '.json'
        if data == nil then
            os.remove(path)
            return
        end
        local f = io.open(path, 'w')
        if f then
            f:write(json.encode(data))
            f:close()
        end
    end
end

function persistence.loadMemory(key)
    ensureDir()
    if useDb then
        local resultId = db.storeQuery("SELECT `data` FROM `monster_memory` WHERE `mem_key` = " .. db.escapeString(key))
        if resultId then
            local data = result.getString(resultId, 'data')
            result.free(resultId)
            if data then
                local ok, t = pcall(json.decode, data)
                if ok then return t end
            end
        end
        return nil
    else
        local path = basePath .. key .. '.json'
        local f = io.open(path, 'r')
        if not f then return nil end
        local content = f:read('*a')
        f:close()
        local ok, t = pcall(json.decode, content)
        if ok then return t end
        return nil
    end
end

return persistence
