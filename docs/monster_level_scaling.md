# Monster Level Scaling

Monsters gain levels based on their spawn depth in the map. Deeper floors create tougher creatures that grant more loot. This system is optional and can be tuned through `config.lua`.

## Configuration

- `enableMonsterLevelScaling` – turn the system on or off.
- `monsterLevelRules` – table describing how many levels each underground floor adds. Each rule contains `minZ`, `maxZ` and `levelsPerFloor` keys.
- `monsterBonusHealth` – extra health per level (percentage).
- `monsterBonusDamage` – extra damage per level (percentage).
- `monsterBonusSpeed` – extra speed per level (percentage).
- `monsterBonusLoot` – extra loot per level (percentage of max count).

Example: with the default rules a monster spawned on floor `Z=12` falls under the third rule. It spawns around level **51–60** and, with `monsterBonusLoot = 0.04`, yields roughly **204%** additional loot.

## Implementation Example

`Spawn::spawnMonster()` assigns the level based on the creature's Z-floor. The formula adds ten levels for every floor below `Z=8`:

```cpp
int baseZ = 8;
int floorOffset = std::abs(pos.getZ() - baseZ);
int minLevel = (floorOffset * 10) + 1;
int maxLevel = minLevel + 9;
uint32_t level = static_cast<uint32_t>(uniform_random(minLevel, maxLevel));
monster_ptr->setLevel(level);
```

For instance, a monster spawned on `Z=10` (two floors below the base) will roll a level in the **21–30** range.

## Using the Level in Scripts

The level is exposed to Lua as `monster:getLevel()`:

```lua
function onDeath(monster, corpse, killer)
    local level = monster:getLevel()
    print(monster:getName() .. " died at level " .. level)
    return true
end
```
