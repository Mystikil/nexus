
local module = {}

function module.run(monster)
  local pos = monster:getPosition()
  local spectators = Game.getSpectators(pos, false, true, 5, 5, 5, 5)
  local players = 0
  for _, m in ipairs(spectators) do
    if m:isPlayer() then
      players = players + 1
    end
  end

  if players >= 2 then
    monster:say("Retreat!", TALKTYPE_MONSTER_SAY)
    monster:teleportTo({x = pos.x + 2, y = pos.y + 2, z = pos.z})
  end
end

return module
