
ECHO = {}

function ECHO.onThink(monster)
  local name = monster:getName():lower()
  local behavior = ECHO_CONFIG[name]
  if behavior then
    local module = require("monster.echo.behaviors." .. behavior)
    module.run(monster)
  end
end
