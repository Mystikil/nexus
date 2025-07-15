-- Placeholder gate break script
-- Called from C++ when a gate expires without being cleared

-- Configure waves per rank. Each entry is a list of monsters with optional count
GateBreakWaves = {
    E = {
        {name = "rat", count = 2}
    },
    D = {
        {name = "orc", count = 3}
    }
}

function onGateBreak(gate)
    -- gate is a table with fields: id, position, rank and type
    print(string.format("Gate %d broke at %d,%d,%d", gate.id, gate.position.x, gate.position.y, gate.position.z))
end
