# Gate Spawner

The gate spawner automatically creates portals at set intervals.
Configuration is done via a Lua table named `GateSpawnConfig` loaded
from `data/scripts/gate/spawn_config.lua`.

```lua
GateSpawnConfig = {
    center = {x = 1000, y = 1000, z = 7},
    radius = 25,            -- tiles around the center
    interval = 5 * 60 * 1000, -- milliseconds
    gates = {
        {rank = GateRank.E, type = GateType.NORMAL, count = 2},
        {rank = GateRank.D, type = GateType.NORMAL, count = 1},
    }
}
```

* **center** – world position used as the spawner origin.
* **radius** – distance from the center in which gates may appear.
* **interval** – time in milliseconds between spawn attempts.
* **gates** – list describing what gates can spawn. Each entry contains:
  * `rank` – gate rank to spawn.
  * `type` – gate type.
  * `count` – how many gates of this rank/type should exist.

The server checks every `interval` and spawns new gates inside the
`radius` when fewer than the specified `count` are present.
