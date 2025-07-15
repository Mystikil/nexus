# Custom Skills System

This server includes a lightweight Lua based skill system that allows adding new skills without recompiling the server.

## Overview

- Skills are defined in **`data/XML/custom_skills.xml`**.
- Player progress is stored in a new MySQL table called **`player_custom_skills`**.
- A Lua library (`data/lib/custom_skills.lua`) loads the XML file and exposes helper
  functions to read or modify a player's skill values.
- Example commands `!mining` and `!gainmining` are provided to check and train the
  sample *Mining* skill.

## Adding Skills

1. Edit `data/XML/custom_skills.xml` and add `<skill>` entries. Each skill has an
   `id`, `name` and `description` attribute.
2. Restart the server so the library reads the new definitions.

## Using Skills

Players can interact with the new skills via Lua scripts. The supplied
`talkaction` demonstrates how to increase a skill and display the value.

- `!mining` &ndash; shows your current Mining level.
- `!gainmining` &ndash; increments Mining by one point.

Developers may call `player:getCustomSkill(id)` or `player:addCustomSkill(id, amount)`
from any Lua script to integrate the skills with game content.

## Storage

Skill values persist in the database table `player_custom_skills` which has the
following fields:

- `player_id` – reference to the player id.
- `skill_id` – id of the custom skill.
- `value` – current skill value.

Entries are automatically created on first access.
