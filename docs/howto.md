# Using the Gate Instance System

This how-to explains different ways you can interact with the gate and instance
system from Lua or through server commands.

## Spawning a Gate Manually
You can create a gate at any position using the global `Game.spawnGate` function:

```lua
local pos = {x = 1000, y = 1000, z = 7}
local gateId = Game.spawnGate(pos, GateRank.C, GateType.NORMAL)
```

This will place a portal at the position and generate a fresh dungeon instance.
A teleport leading to the instance entrance is automatically created on the tile.

## Removing a Gate
If you need to remove a gate before it expires:

```lua
Game.removeGate(gateId)
```

This cleans up the instance tiles and monsters immediately.

## Customising Gate Behaviour
- **GateBreak Waves**: Define the table `GateBreakWaves` in a Lua script under
  `data/scripts/gate/` to spawn monsters when an uncleared gate breaks.
- **onGateBreak Callback**: Implement a function `onGateBreak(gate)` to run custom
  logic when a gate collapses.

## Example: Event Trigger
You can tie gate spawning to other events, such as defeating a boss or talking
to an NPC. For example:

```lua
function onBossDefeated(player)
    local gatePos = player:getPosition()
    Game.spawnGate(gatePos, GateRank.B, GateType.RED)
end
```

## Extending the System
The instance generator is intentionally simple. You can modify the C++ code or
extend it via Lua scripts to introduce new room shapes, hazards or puzzles.

* Use `Instance::generateLayout` to build more complex maps.
* Spawn extra creatures or NPCs inside `Instance::spawnMonsters` or through Lua
  after the gate is created.
* Place rewards or quest objectives using the returned entrance and exit points.

This flexibility allows the gate mechanic to serve different purposes such as
time-limited challenge dungeons, story encounters or seasonal events.
