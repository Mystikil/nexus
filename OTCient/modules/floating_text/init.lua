Module.name = "floating_text"
Module.description = "Displays floating combat numbers"
Module.version = "1.0"

function init()
    connect(g_game, {
        onTextMessage = floatingText.onTextMessage
    })
end

function terminate()
    disconnect(g_game, {
        onTextMessage = floatingText.onTextMessage
    })
end

function floatingText.onTextMessage(mode, text)
    local localPlayer = g_game.getLocalPlayer()
    if not localPlayer then return end

    local pos = localPlayer:getPosition()
    local color = TextColors[mode] or TextColors[MessageModes.Failure]
    addAnimatedText(pos, color, text)
end

-- Default color mapping (adjust as needed)
TextColors = {
    [MessageModes.DamageDealt] = 'red',
    [MessageModes.DamageReceived] = 'orange',
    [MessageModes.Heal] = 'green'
}
