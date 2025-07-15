local keywordHandler = KeywordHandler:new()
local npcHandler = NpcHandler:new(keywordHandler)
NpcSystem.parseParameters(npcHandler)

local Handler = dofile('data/scripts/crafting/handler.lua')
dofile('data/scripts/crafting/recipes/weapons.lua')

local crafting = Handler.create(npcHandler, weaponRecipes, PlayerStorageKeys.weaponsmithSkill)

function onCreatureAppear(cid) npcHandler:onCreatureAppear(cid) end
function onCreatureDisappear(cid) npcHandler:onCreatureDisappear(cid) end
function onCreatureSay(cid, type, msg) npcHandler:onCreatureSay(cid, type, msg) end
function onThink() npcHandler:onThink() end

npcHandler:setCallback(CALLBACK_MESSAGE_DEFAULT, crafting.callback)
npcHandler:addModule(FocusModule:new())
