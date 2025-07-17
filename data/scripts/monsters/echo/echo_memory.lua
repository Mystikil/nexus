
ECHO_MEMORY = {}

function ECHO_MEMORY.remember(monster, key, value)
  local id = monster:getId()
  ECHO_MEMORY[id] = ECHO_MEMORY[id] or {}
  ECHO_MEMORY[id][key] = value
end

function ECHO_MEMORY.recall(monster, key)
  local id = monster:getId()
  return ECHO_MEMORY[id] and ECHO_MEMORY[id][key]
end
