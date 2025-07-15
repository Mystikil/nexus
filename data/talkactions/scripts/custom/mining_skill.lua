local SKILL_ID = 1 -- Mining skill id defined in custom_skills.xml

function onSay(player, words, param)
    if words == '!gainmining' then
        local newValue = player:addCustomSkill(SKILL_ID, 1)
        player:sendTextMessage(MESSAGE_STATUS_SMALL, 'Mining increased to ' .. newValue .. '.')
    else
        local level = player:getCustomSkill(SKILL_ID)
        player:sendTextMessage(MESSAGE_STATUS_SMALL, 'Mining level: ' .. level .. '.')
    end
    return false
end
