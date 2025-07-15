local talkAction = TalkAction("!skills")

function talkAction.onSay(player, words, param)
	local message = ""
	for id, info in pairs(CustomSkills.skills) do
		local level = player:getCustomSkill(id)
		message = message .. info.name .. ": " .. level .. "\n"
	end

	local window = ModalWindow{
		title = "Custom Skills",
		message = message
	}
	window:addButton("Ok")
	window:setDefaultEnterButton("Ok")
	window:setDefaultEscapeButton("Ok")
	window:sendToPlayer(player)

	return false
end

talkAction:separator(" ")
talkAction:register()
