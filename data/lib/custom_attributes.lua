CustomAttributes = {}
CustomAttributes.attributes = {}

function CustomAttributes.load()
    local path = 'data/XML/custom_attributes.xml'
    for line in io.lines(path) do
        local id = tonumber(line:match('id="(%d+)"'))
        if id then
            local name = line:match('name="([^"]+)"') or ''
            local desc = line:match('description="([^"]+)"') or ''
            local group = line:match('group="([^"]+)"') or ''
            CustomAttributes.attributes[id] = {name = name, description = desc, group = group}
        end
    end
end

function CustomAttributes.getName(id)
    local info = CustomAttributes.attributes[id]
    return info and info.name or 'Unknown'
end

function CustomAttributes.getDescription(id)
    local info = CustomAttributes.attributes[id]
    return info and info.description or ''
end

function CustomAttributes.getGroup(id)
    local info = CustomAttributes.attributes[id]
    return info and info.group or ''
end
