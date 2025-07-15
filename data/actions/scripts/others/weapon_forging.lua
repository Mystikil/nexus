-- weapon_forging.lua (Final version with category-safe attribute application)
dofile('data/lib/rarity_config.lua')
local forgeTiers = {
  [1] = {
    name = "Uncommon",
    successRate = 90,
    bonus = {
      weapon = { attack = 2, hitchance = 4 },
      armor = { defense = 2, armor = 1 },
      ring = { manaRegen = 1 },
      magic = { magicLevelPoints = 1 }
    },
    effect = CONST_ME_MAGIC_GREEN
  },
  [2] = {
    name = "Rare",
    successRate = 75,
    bonus = {
      weapon = { attack = 4, hitchance = 6, elementType = COMBAT_FIREDAMAGE, elementDamage = 8 },
      armor = { defense = 3, armor = 3 },
      ring = { manaRegen = 2, healthRegen = 1 },
      magic = { magicLevelPoints = 2, manaLeechChance = 2 }
    },
    effect = CONST_ME_FIREAREA
  },
  [3] = {
    name = "Magical",
    successRate = 60,
    bonus = {
      weapon = { attack = 6, hitchance = 8, elementType = COMBAT_ENERGYDAMAGE, elementDamage = 12 },
      armor = { defense = 4, armor = 4, reflectPercentAll = 3 },
      ring = { manaRegen = 3, speedBonus = 5 },
      magic = { magicLevelPoints = 3, lifeLeechChance = 3 }
    },
    effect = CONST_ME_ENERGYAREA
  }
}

local crystalTiers = {
  [2145] = 1, -- Diamond
  [2147] = 2, -- Ruby
  [2150] = 3  -- Grey Gem
}

local weaponTypes = { WEAPON_SWORD, WEAPON_CLUB, WEAPON_AXE, WEAPON_DISTANCE }
local bit = bit32 or bit

local function selectRarity()
  local total = 0
  for i = 1, #RARITY_CONFIG do
    total = total + RARITY_CONFIG[i].chance
  end
  local roll = math.random(1, total)
  local sum = 0
  for i = 1, #RARITY_CONFIG do
    sum = sum + RARITY_CONFIG[i].chance
    if roll <= sum then
      return RARITY_CONFIG[i]
    end
  end
  return RARITY_CONFIG[#RARITY_CONFIG]
end

local function getItemCategory(item)
  local it = item:getType()
  local slotPos = it:getSlotPosition()
  local weaponType = it:getWeaponType()

  if table.contains(weaponTypes, weaponType) then
    return "weapon"
  elseif bit.band(slotPos, CONST_SLOT_HEAD) > 0 or bit.band(slotPos, CONST_SLOT_ARMOR) > 0 or bit.band(slotPos, CONST_SLOT_LEGS) > 0 then
    return "armor"
  elseif bit.band(slotPos, CONST_SLOT_RING) > 0 or bit.band(slotPos, CONST_SLOT_NECKLACE) > 0 then
    return "ring"
  elseif weaponType == WEAPON_WAND or weaponType == WEAPON_ROD then
    return "magic"
  else
    return nil
  end
end

local function getElementName(type)
  local names = {
    [COMBAT_FIREDAMAGE] = "Fire",
    [COMBAT_ICEDAMAGE] = "Ice",
    [COMBAT_EARTHDAMAGE] = "Earth",
    [COMBAT_ENERGYDAMAGE] = "Energy",
    [COMBAT_HOLYDAMAGE] = "Holy"
  }
  return names[type] or "Neutral"
end

local function safeSetAttribute(item, attr, value)
  if item and item:isItem() and value ~= nil then
    item:setAttribute(attr, value)
  end
end

function onUse(player, item, fromPos, target, toPos, isHotkey)
  if not player or not item or not item:isItem() or not target or not target:isItem() then
    return false
  end

  local itemType = target:getType()
  if not itemType:isMovable() then
    player:sendCancelMessage("This item cannot be forged.")
    return false
  end

  local weaponType = itemType:getWeaponType()
  if weaponType == WEAPON_AMMO or weaponType == WEAPON_WAND or weaponType == WEAPON_ROD then
    player:sendCancelMessage("You can't forge this type of item.")
    return false
  end

  local tier = crystalTiers[item:getId()]
  local tierData = forgeTiers[tier]
  if not tierData then
    player:sendCancelMessage("This crystal cannot be used for forging.")
    return true
  end

  local category = getItemCategory(target)
  if not category then
    player:sendCancelMessage("This item cannot be forged.")
    return true
  end

  local bonus = tierData.bonus[category]
  if not bonus then
    player:sendCancelMessage("No bonuses defined for this item type.")
    return true
  end

  -- Store original values
  local originalAttack = target:getAttack()
  local originalDefense = target:getDefense()
  local originalArmor = target:getArmor()
  local targetPos = target:getPosition()

  local success = math.random(1, 100) <= tierData.successRate

  target:remove(1)
  item:remove(1)

  if not success then
    player:addItem(2148, 100)
    targetPos:sendMagicEffect(CONST_ME_EXPLOSIONHIT)
    player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Forging failed! The item shattered.")
    return true
  end

  local newItem = player:addItem(itemType:getId(), 1)
  if not newItem then
    player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Forging succeeded, but the item could not be returned.")
    return true
  end

  -- Determine rarity and apply multipliers
  local rarity = selectRarity()
  local multiplier = rarity.bonus or 1

  local attack = originalAttack + (bonus.attack or 0)
  local defense = originalDefense + (bonus.defense or 0)
  local armor = originalArmor + (bonus.armor or 0)
  local elementDamage = bonus.elementDamage and math.floor(bonus.elementDamage * multiplier)

  attack = math.floor(attack * multiplier)
  defense = math.floor(defense * multiplier)
  armor = math.floor(armor * multiplier)

  if category == "weapon" then
    safeSetAttribute(newItem, ITEM_ATTRIBUTE_ATTACK, attack)
    safeSetAttribute(newItem, ITEM_ATTRIBUTE_HITCHANCE, bonus.hitchance)
    if bonus.elementType and elementDamage then
      safeSetAttribute(newItem, ITEM_ATTRIBUTE_ELEMENTTYPE, bonus.elementType)
      safeSetAttribute(newItem, ITEM_ATTRIBUTE_ELEMENTDAMAGE, elementDamage)
    end
  elseif category == "armor" then
    safeSetAttribute(newItem, ITEM_ATTRIBUTE_DEFENSE, defense)
    safeSetAttribute(newItem, ITEM_ATTRIBUTE_ARMOR, armor)
  end

  local coloredName = string.format("\\#%s%s %s\\#", rarity.color, rarity.name, itemType:getName())
  newItem:setAttribute(ITEM_ATTRIBUTE_NAME, coloredName)
  newItem:setAttribute(ITEM_ATTRIBUTE_DESCRIPTION, rarity.description)
  newItem:setActionId(4500 + tier)

  player:getPosition():sendMagicEffect(tierData.effect)
  player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Forging successful!")
  return true
end
