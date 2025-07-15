# Gate Spawner

The gate spawner automatically creates portals at set intervals.
Configuration is done via a Lua table named `GateSpawnConfig` loaded
from `data/scripts/gate/spawn_config.lua`.

```lua
GateSpawnConfig = {
    center = {x = 1000, y = 1000, z = 7},
    radius = 25,      -- tiles around the center
    interval = 60000, -- milliseconds between spawn checks
    rules = {
        {rank = GateRank.E, type = GateType.NORMAL, max = 2},
        {rank = GateRank.D, type = GateType.RED,    max = 1}
    }
}
```

* **center** – world position used as the spawner origin.
* **radius** – distance from the center in which gates may appear.
* **interval** – time in milliseconds between spawn attempts.
* **rules** – list describing what gates can spawn. Each rule contains:
  * `rank` – gate rank to spawn.
  * `type` – gate type.
  * `max`  – maximum number of gates of this rank/type that may exist.

The server checks every `interval` and spawns new gates inside the
`radius` when fewer than `max` are present.
