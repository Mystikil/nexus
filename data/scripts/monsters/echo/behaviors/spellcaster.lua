
local module = {}

function module.run(monster)
  local target = monster:getTarget()
  if target then
    if math.random(100) <= 50 then
      monster:say("Arcane blast!", TALKTYPE_MONSTER_SAY)
      monster:castSpell("energy wave")
    end
  end
end

return module
