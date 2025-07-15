-- Shared crafting logic for NPC professions
-- Designed for reuse across weapon smithing and future professions.
local Crafting = {}

-- Retrieve a player's skill value stored in the provided storage key
function Crafting.getSkill(player, storage)
    local value = player:getStorageValue(storage)
    if value < 0 then
        value = 0
    end
    return value
end

-- Increase a player's skill value by the given amount (default 1)
function Crafting.addSkillPoint(player, storage, amount)
    amount = amount or 1
    player:setStorageValue(storage, Crafting.getSkill(player, storage) + amount)
end

-- Check if the player possesses all required items
function Crafting.hasRequirements(player, requirements)
    for _, req in ipairs(requirements) do
        if player:getItemCount(req.id) < req.count then
            return false
        end
    end
    return true
end

-- Remove the required items from the player
function Crafting.consumeRequirements(player, requirements)
    for _, req in ipairs(requirements) do
        player:removeItem(req.id, req.count)
    end
end

-- Generate a user friendly requirements description
function Crafting.getRequirementsText(recipe)
    local parts = {}
    for _, req in ipairs(recipe.requirements) do
        local name = ItemType(req.id):getName()
        table.insert(parts, req.count .. " " .. name .. (req.count > 1 and "s" or ""))
    end
    if recipe.cost and recipe.cost > 0 then
        table.insert(parts, recipe.cost .. " gold coins")
    end
    return table.concat(parts, ", ")
end

-- Calculate the success chance based on player level and skill
function Crafting.getSuccessChance(player, skill)
    local chance = 50 + skill * 5 + math.floor(player:getLevel() / 10)
    local mastery = player.getCustomAttribute and player:getCustomAttribute(27) or 0
    if mastery > 0 then
        chance = chance + mastery
    end
    if chance > 95 then
        chance = 95
    end
    return chance
end

return Crafting
