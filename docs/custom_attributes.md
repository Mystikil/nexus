## Overview

- **Definitions:** `data/XML/custom_attributes.xml`
- **Loader:** `data/lib/custom_attributes.lua`
- **Initialization:** `CustomAttributes.load()` is called from `data/global.lua`

Each attribute entry provides a name, category group and description. Scripts can read these details using the helper functions exposed by the library.

## Attribute List

The default configuration defines the following attributes:

| ID | Name                  | Group   | Description                                      |
|----|-----------------------|---------|--------------------------------------------------|
| 1  | Armor Penetration     | combat  | Ignores a portion of target's armor              |
| 2  | Block Chance          | combat  | Flat chance to negate incoming damage            |
| 3  | Parry Chance          | combat  | Reduces damage from melee hits                   |
| 4  | Dodge Chance          | combat  | Chance to completely avoid an attack             |
| 5  | Reflect Damage        | combat  | Returns percentage of damage to attacker         |
| 6  | True Damage           | combat  | Ignores all resistances and defenses             |
| 7  | Bleed Chance          | combat  | Causes target to take damage over time           |
| 8  | Execution Chance      | combat  | Bonus damage versus low HP targets               |
| 9  | Knockback Strength    | combat  | Pushes enemies back on hit                       |
| 10 | Stun Chance           | combat  | Temporarily prevents enemy movement and actions  |
| 11 | Spell Power           | magic   | Increases magic damage and healing               |
| 12 | Mana Efficiency       | magic   | Reduces mana cost of spells                      |
| 13 | Silence Resistance    | magic   | Reduces chance of being silenced                 |
| 14 | Holy Vulnerability    | magic   | Increases damage taken from holy sources         |
| 15 | Elemental Surge Chance| magic   | Small chance for extra elemental hit             |
| 16 | Curse Resistance      | magic   | Reduces the effect or duration of curses         |
| 17 | Chaos Affinity        | magic   | Boosts mixed-damage spells                       |
| 18 | Weight Reduction      | utility | Carry more before becoming slow                  |
| 19 | XP Gain Bonus         | utility | Increases experience earned from kills           |
| 20 | Loot Drop Bonus       | utility | Increases drop chance of rare items              |
| 21 | Trap Resistance       | utility | Less damage or avoidance from traps              |
| 22 | Aggro Modifier        | utility | Attracts or repels enemy targeting               |
| 23 | Jump Distance         | utility | Enables short dashes or blinks                   |
| 24 | Auto Heal Chance      | utility | Chance to trigger small heal on damage taken     |
| 25 | Status Duration Reduction | utility | Shortens poison, burn and freeze durations    |
| 26 | Pet Damage Bonus      | utility | Boosts damage done by summoned creatures         |
| 27 | Crafting Mastery      | utility | Increases yield or success in professions        |
| 28 | Night Vision          | utility | Enhances visibility in dark areas                |
| 29 | Burning Aura          | utility | Deals small fire damage to nearby enemies        |
| 30 | Resourceful           | utility | Small chance to not consume runes or potions     |

---

## Using Attributes

Scripts may access the definitions as follows:

```lua
local name = CustomAttributes.getName(1) -- "Armor Penetration"
local desc = CustomAttributes.getDescription(1)
local group = CustomAttributes.getGroup(1)
