CustomSkills = {}
CustomSkills.skills = {}

function CustomSkills.load()
    local path = 'data/XML/custom_skills.xml'
    for line in io.lines(path) do
        local id = tonumber(line:match('id="(%d+)"'))
        if id then
            local name = line:match('name="([^"]+)"') or ''
            local desc = line:match('description="([^"]*)"') or ''
            CustomSkills.skills[id] = {name = name, description = desc}
        end
    end
end

local function ensureEntry(playerId, skillId)
    db.query('INSERT IGNORE INTO `player_custom_skills` (`player_id`,`skill_id`,`value`) VALUES (' .. playerId .. ',' .. skillId .. ',0)')
end

function Player.getCustomSkill(self, skillId)
    ensureEntry(self:getGuid(), skillId)
    local resultId = db.storeQuery('SELECT `value` FROM `player_custom_skills` WHERE `player_id`=' .. self:getGuid() .. ' AND `skill_id`=' .. skillId)
    if resultId then
        local value = result.getNumber(resultId, 'value')
        result.free(resultId)
        return value
    end
    return 0
end

function Player.addCustomSkill(self, skillId, amount)
    local value = self:getCustomSkill(skillId) + amount
    db.query('UPDATE `player_custom_skills` SET `value`=' .. value .. ' WHERE `player_id`=' .. self:getGuid() .. ' AND `skill_id`=' .. skillId)
    return value
end

function CustomSkills.getSkillName(skillId)
    local info = CustomSkills.skills[skillId]
    return info and info.name or 'Unknown'
end

function CustomSkills.getSkillDescription(skillId)
    local info = CustomSkills.skills[skillId]
    return info and info.description or ''
end
