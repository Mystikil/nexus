# Gate Instance System Overview

This document describes the experimental **Gate Instance System** built on top of The Forgotten Server.
It allows gates in the world to open portals into **procedurally generated dungeons**. Each gate spawns
its own instance containing custom tiles, monsters and an entrance/exit pair.

## Components

### Gate
- Tracks the portal that appears in the world
- Fields include `id`, `position`, `rank`, `type`, timestamps and flags
- Now also stores a pointer to an `Instance` representing the dungeon

### Instance
- Represents one dynamically created dungeon
- Holds an origin position where the dungeon is pasted
- Generates a 2D grid describing floor, walls and spawn locations
- Places temporary `Tile` objects and spawns monsters
- Cleans up everything when the gate expires or is removed

### DungeonGenerator
- Utility that builds a simple layout based on `GateRank`
- Currently creates a rectangular room surrounded by walls
- Picks monster positions at random and chooses an entrance/exit spot
- Returns the grid and relative coordinates to the `Instance`

### GateManager
- Creates gates with `spawnGate(position, rank, type)`
- When spawning a gate it allocates a new `Instance`, generates the layout,
  places tiles and spawns monsters
- Adds a `Teleport` item on the gate tile that leads to the instance entry point
- During `update()` it checks expiration and removes gates
  - If a gate breaks uncleared it calls the optional Lua hook `onGateBreak`
  - Spawns configured break waves and cleans up the associated `Instance`

### Teleport Logic
A teleport (item id `1387`) is inserted on the gate tile. Stepping on it transports
players to the instance `entryPoint`. Exits inside the dungeon can be handled with
normal teleport tiles or scripts.

## TODO
- Expand `DungeonGenerator` to create multiple rooms and corridors
- Support different themes and random props
- Record dungeon layouts to logs for easier debugging
- Add Lua hooks/events for when an instance is cleared
- Handle double gates and linked instances

This system is still early in development but provides a foundation for dynamic
encounters that scale with gate rank.
