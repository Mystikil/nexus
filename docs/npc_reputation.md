# NPC Reputation System

This project includes a Lua based reputation system that allows NPCs to track how much each player has interacted with them. Gaining reputation unlocks cheaper shop prices, small gifts and access to special quests.

## Overview

- **Library:** `data/npc/lib/reputation.lua`
- **Configuration:** `data/npc/reputation_config.xml`
- **NPC Integration:** modifications in `data/npc/lib/npcsystem/modules.lua` and `data/npc/lib/npcsystem/npchandler.lua`

The reputation library is loaded automatically when NPC scripts are executed. Each NPC shares the same configuration but tracks reputation for players individually.

### XML Configuration

```xml
<?xml version="1.0" encoding="UTF-8"?>
<reputation gain="1">
    <threshold name="discount" value="10" />
    <threshold name="gift" value="20" />
    <threshold name="quest" value="30" />
</reputation>
```

- **gain** – amount of reputation awarded each time a player interacts with an NPC (greet or trade).
- **threshold "discount"** – reputation required before shop prices are reduced by 20%.
- **threshold "gift"** – reputation required for the NPC to give a small reward (10 platinum coins).
- **threshold "quest"** – reputation required before the NPC offers a unique quest.

Adjust these values to tune your server. The configuration is loaded when the server starts.

### Reputation Storage

Reputation values are stored in player storage. A unique storage key is derived from the NPC name, ensuring each NPC keeps its own reputation score for every player. Reputation increases when:

1. A player greets an NPC for the first time in a session.
2. A player completes a shop transaction.

The `priceModifier` function in the library applies the discount once the threshold is reached.

### Gift and Quest Unlocks

When a player reaches the `gift` threshold, the NPC automatically hands out 10 platinum coins (once). At the `quest` threshold the NPC informs the player about a rare quest. These checks occur when greeting the NPC.

### Adding New Behavior

Developers can call the functions exposed by the library from any NPC script:

- `Reputation.add(player, npcName)` – increase the player's reputation with the NPC.
- `Reputation.get(player, npcName)` – read the current reputation value.
- `Reputation.priceModifier(player, npcName)` – return 0.8 if the player receives a discount, otherwise 1.

Feel free to expand the library with additional rewards or NPC dialogue.

## Usage

1. Place `reputation.lua` in `data/npc/lib/` and `reputation_config.xml` in `data/npc/`.
2. Ensure NPC scripts include the modified `modules.lua` and `npchandler.lua` from this repository.
3. Restart the server. NPCs will automatically grant reputation when greeted or when a purchase is made.

Players can improve relations with any NPC simply by interacting with them. Over time they will receive discounts, small gifts and special quests based on the thresholds you configure.

