local EchoAI = dofile('data/scripts/monsters/echo_base.lua')
local event = Event()

function event.onSpawn(self, position, startup, artificial)
    if self:getName():lower() ~= 'echo golem' then
        return true
    end
    return EchoAI.onSpawn(self, position, startup, artificial)
end

event:register()
