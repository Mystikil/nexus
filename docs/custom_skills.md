# Custom Skills System

This server includes a lightweight Lua based skill system that allows adding new skills without recompiling the server.

## Overview

- Skills are defined in **`data/XML/custom_skills.xml`**.
- Player progress is stored in a new MySQL table called **`player_custom_skills`**.
- A Lua library (`data/lib/custom_skills.lua`) loads the XML file and exposes helper
  functions to read or modify a player's skill values.
- Example commands `!mining` and `!gainmining` are provided to check and train the
  sample *Mining* skill.

The default configuration includes several sample skills that can be extended:

- **Mining** – extract ore from rocks.
- **Woodcutting** – chop wood from trees.
- **Fishing** – catch fish from water sources.
- **Smithing** – craft weapons and armor.
- **Cooking** – prepare tasty meals.
- **Fletching** – craft bows and arrows.
- **Herbalism** – harvest herbs for potions.

## Adding Skills

1. Edit `data/XML/custom_skills.xml` and add `<skill>` entries. Each skill has an
   `id`, `name` and `description` attribute.
2. Restart the server so the library reads the new definitions.

## Using Skills

Players can interact with the new skills via Lua scripts. The supplied
`talkaction` demonstrates how to view a skill value, but experience is now earned
by using the appropriate tool on special resource nodes.

- `!mining` &ndash; shows your current Mining level.

To grant experience, define mining nodes in `data/lib/mining_nodes.lua` and make
players use a pick (item id `2553`) on those nodes. Each node entry specifies the
skill id, experience gained and a loot table with drop chances. When the node is
mined the loot is added to the player's inventory and the node is removed.

Developers may call `player:getCustomSkill(id)` or `player:addCustomSkill(id, amount)`
from any Lua script to integrate the skills with game content. The pickaxe action
already calls `addCustomSkill` automatically when mining nodes are used.

## Storage

Skill values persist in the database table `player_custom_skills` which has the
following fields:

- `player_id` – reference to the player id.
- `skill_id` – id of the custom skill.
- `value` – current skill value.

Entries are automatically created on first access.
