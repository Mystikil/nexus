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

## Using the Level in Scripts

The level is exposed to Lua as `monster:getLevel()`:

```lua
function onDeath(monster, corpse, killer)
    local level = monster:getLevel()
    print(monster:getName() .. " died at level " .. level)
    return true
end
```
