-- NPC crafting handler providing a callback for NpcHandler
-- This module is reusable for any profession that utilises Crafting recipes.

local Crafting = dofile('data/scripts/crafting/core.lua')
local Handler = {}

-- Create a new handler instance
-- @param npcHandler the npc handler controlling speech
-- @param recipes table of crafting recipes
-- @param storage storage value used for the player's skill
function Handler.create(npcHandler, recipes, storage)
    local self = {
        npcHandler = npcHandler,
        recipes = recipes,
        storage = storage,
        topics = {}
    }

    -- message callback used with NpcHandler
    function self.callback(cid, _type, msg)
        if not self.npcHandler:isFocused(cid) then
            return false
        end

        local player = Player(cid)
        local text = msg:lower()
        local recipe = self.recipes[text]

        if recipe then
            local req = Crafting.getRequirementsText(recipe)
            if recipe.level then
                req = req .. " and level " .. recipe.level
            end
            self.npcHandler:say("To craft a " .. text .. " you need " .. req .. ". Do you want me to craft it?", cid)
            self.topics[cid] = text
            return true
        elseif text == "skill" then
            local skill = Crafting.getSkill(player, self.storage)
            self.npcHandler:say("Your weaponsmith skill is " .. skill .. ".", cid)
            return true
        elseif text == "craft" or text == "crafts" then
            local levels = {}
            for _, recipe in pairs(self.recipes) do
                if recipe.level and not table.contains(levels, recipe.level) then
                    table.insert(levels, recipe.level)
                end
            end
            table.sort(levels)
            if #levels > 0 then
                local levelText = {}
                for _, lvl in ipairs(levels) do
                    table.insert(levelText, tostring(lvl))
                end
                self.npcHandler:say("I craft items for these levels: " .. table.concat(levelText, ", ") .. ".", cid)
            else
                self.npcHandler:say("I currently have no crafting recipes.", cid)
            end
            return true
        elseif text:match("^level %d+ crafts$") then
            local lvl = tonumber(text:match("^level (%d+)"))
            local names = {}
            for name, recipe in pairs(self.recipes) do
                if recipe.level == lvl then
                    table.insert(names, name)
                end
            end
            table.sort(names)
            if #names > 0 then
                self.npcHandler:say("At level " .. lvl .. " you can craft: " .. table.concat(names, ", ") .. ".", cid)
            else
                self.npcHandler:say("I don't have any crafts for level " .. lvl .. ".", cid)
            end
            return true
        elseif text == "yes" and self.topics[cid] then
            recipe = self.recipes[self.topics[cid]]
            if player:getLevel() < (recipe.level or 0) then
                self.npcHandler:say("You need to be level " .. recipe.level .. " to craft this.", cid)
            elseif player:getMoney() < (recipe.cost or 0) then
                self.npcHandler:say("You don't have enough gold.", cid)
            elseif not Crafting.hasRequirements(player, recipe.requirements) then
                self.npcHandler:say("You lack the required items.", cid)
            else
                local skill = Crafting.getSkill(player, self.storage)
                local chance = Crafting.getSuccessChance(player, skill)
                Crafting.consumeRequirements(player, recipe.requirements)
                if recipe.cost and recipe.cost > 0 then
                    player:removeMoney(recipe.cost)
                end
                if math.random(100) <= chance then
                    player:addItem(recipe.result, 1)
                    Crafting.addSkillPoint(player, self.storage)
                    self.npcHandler:say("Success! Here is your " .. ItemType(recipe.result):getName() .. ".", cid)
                else
                    self.npcHandler:say("The attempt failed. Try again later.", cid)
                end
            end
            self.topics[cid] = nil
            return true
        elseif text == "no" and self.topics[cid] then
            self.npcHandler:say("Maybe another time.", cid)
            self.topics[cid] = nil
            return true
        end
        return true
    end

    return self
end

return Handler
