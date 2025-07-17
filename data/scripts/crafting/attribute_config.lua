-- Default chances for weapon attributes when crafting
-- id = percentage chance to apply

-- Percentages doubled compared to the original defaults to make bonuses
-- appear more frequently. If no attribute is applied during crafting the
-- handler will reroll until one succeeds.
WeaponAttributeConfig = {
    [1] = 10, -- Armor Penetration
    [2] = 8,  -- Block Chance
    [3] = 8,  -- Parry Chance
    [4] = 6,  -- Dodge Chance
    [5] = 4,  -- Reflect Damage
    [6] = 6,  -- True Damage
    [7] = 4,  -- Bleed Chance
    [8] = 2,  -- Execution Chance
    [9] = 4,  -- Knockback Strength
    [10] = 2, -- Stun Chance
    [11] = 6, -- Spell Power
    [12] = 6, -- Mana Efficiency
    [13] = 4, -- Silence Resistance
    [14] = 2, -- Holy Vulnerability
    [15] = 4, -- Elemental Surge Chance
    [16] = 4, -- Curse Resistance
    [17] = 2, -- Chaos Affinity
    [18] = 4, -- Weight Reduction
    [19] = 8, -- XP Gain Bonus
    [20] = 4, -- Loot Drop Bonus
    [21] = 4, -- Trap Resistance
    [22] = 6, -- Aggro Modifier
    [23] = 2, -- Jump Distance
    [24] = 4, -- Auto Heal Chance
    [25] = 4, -- Status Duration Reduction
    [26] = 2, -- Pet Damage Bonus
    [27] = 6, -- Crafting Mastery
    [28] = 2, -- Night Vision
    [29] = 4, -- Burning Aura
    [30] = 4  -- Resourceful
}
