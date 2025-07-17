
local module = {}

function module.run(monster)
  local pos = monster:getPosition()
  local spectators = Game.getSpectators(pos, false, true, 5, 5, 5, 5)
  local allies = 0
  for _, m in ipairs(spectators) do
    if m:isMonster() and m:getName() == monster:getName() then
      allies = allies + 1
    end
  end

  if allies >= 3 then
    monster:say("We are many!", TALKTYPE_MONSTER_SAY)
    monster:selectTarget()
  end
end

return module
