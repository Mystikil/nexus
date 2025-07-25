-- Combat settings
-- NOTE: valid values for worldType are: "pvp", "no-pvp" and "pvp-enforced"
worldType = "pvp"
hotkeyAimbotEnabled = true
protectionLevel = 1
killsToRedSkull = 3
killsToBlackSkull = 6
pzLocked = 60000
removeChargesFromRunes = true
removeChargesFromPotions = true
removeWeaponAmmunition = true
removeWeaponCharges = true
timeToDecreaseFrags = 24 * 60 * 60
whiteSkullTime = 15 * 60
stairJumpExhaustion = 2000
experienceByKillingPlayers = false
expFromPlayersLevelRange = 75

-- Connection Config
-- NOTE: maxPlayers set to 0 means no limit
-- NOTE: allowWalkthrough is only applicable to players
ip = "127.0.0.1"
bindOnlyGlobalAddress = false
loginProtocolPort = 7271
gameProtocolPort = 7172
statusProtocolPort = 7171
maxPlayers = 1000
motd = "Welcome to The Nexus Server!"
onePlayerOnlinePerAccount = true
allowClones = false
allowWalkthrough = true
serverName = "Nexus"
statusTimeout = 5000
replaceKickOnLogin = true
maxPacketsPerSecond = 25

-- Pathfinding
-- pathfindingInterval handles how often paths are force drawn
-- pathfindingDelay delays any recently drawn paths from drawing again
-- pathfindingDelay does not delay pathfindingInterval
pathfindingInterval = 200
pathfindingDelay = 300

-- Deaths
-- NOTE: Leave deathLosePercent as -1 if you want to use the default
-- death penalty formula. For the old formula, set it to 10. For
-- no skill/experience loss, set it to 0.
deathLosePercent = -1

-- Houses
-- NOTE: set housePriceEachSQM to -1 to disable the ingame buy house functionality
-- NOTE: valid values for houseRentPeriod are: "daily", "weekly", "monthly", "yearly"
-- use any other value to disable the rent system
housePriceEachSQM = 1000
houseRentPeriod = "never"
houseOwnedByAccount = false
houseDoorShowPrice = true
onlyInvitedCanMoveHouseItems = true

-- Item Usage
timeBetweenActions = 200
timeBetweenExActions = 1000

-- Map
-- NOTE: set mapName WITHOUT .otbm at the end
mapName = "forgotten"
mapAuthor = "Komic"

-- Market
marketOfferDuration = 30 * 24 * 60 * 60
premiumToCreateMarketOffer = true
checkExpiredMarketOffersEachMinutes = 60
maxMarketOffersAtATimePerPlayer = 100

mysqlHost = "127.0.0.1"
mysqlUser = "root"
mysqlPass = ""
mysqlDatabase = "virellion"
mysqlPort = 3306
mysqlSock = ""

-- Misc.
-- NOTE: classicAttackSpeed set to true makes players constantly attack at regular
-- intervals regardless of other actions such as item (potion) use. This setting
-- may cause high CPU usage with many players and potentially affect performance!
-- NOTE: forceMonsterTypesOnLoad loads all monster types on startup to validate them.
-- You can disable it to save some memory if you don't see any errors at startup.
-- checkDuplicateStorageKeys checks the values stored in the variables for duplicates.
allowChangeOutfit = true
freePremium = false
kickIdlePlayerAfterMinutes = 15
maxMessageBuffer = 4
emoteSpells = false
classicEquipmentSlots = false
classicAttackSpeed = false
showScriptsLogInConsole = false
showOnlineStatusInCharlist = false
yellMinimumLevel = 2
yellAlwaysAllowPremium = false
minimumLevelToSendPrivate = 1
premiumToSendPrivate = false
forceMonsterTypesOnLoad = true
cleanProtectionZones = false
showPlayerLogInConsole = true
checkDuplicateStorageKeys = false

-- VIP and Depot limits
-- NOTE: you can set custom limits per group in data/XML/groups.xml
vipFreeLimit = 20
vipPremiumLimit = 100
depotFreeLimit = 2000
depotPremiumLimit = 10000

-- World Light
-- NOTE: if defaultWorldLight is set to true the world light algorithm will
-- be handled in the sources. set it to false to avoid conflicts if you wish
-- to make use of the function setWorldLight(level, color)
defaultWorldLight = true

-- Server Save
-- NOTE: serverSaveNotifyDuration in minutes
serverSaveNotifyMessage = true
serverSaveNotifyDuration = 5
serverSaveCleanMap = false
serverSaveClose = false
serverSaveShutdown = true

-- Experience stages
-- NOTE: to use a flat experience multiplier, set experienceStages to nil
-- minlevel and multiplier are MANDATORY
-- maxlevel is OPTIONAL, but is considered infinite by default
-- to disable stages, create a stage with minlevel 1 and no maxlevel
experienceStages = {
	{ minlevel = 1, maxlevel = 8, multiplier = 200 },
	{ minlevel = 9, maxlevel = 20, multiplier = 150 },
	{ minlevel = 21, maxlevel = 50, multiplier = 100 },
	{ minlevel = 51, maxlevel = 100, multiplier = 50 },
	{ minlevel = 101, multiplier = 25 },
  { minlevel = 200, multiplier = 20 },
  { minlevel = 300, multiplier = 15 },
  { minlevel = 400, multiplier = 10 },
  { minlevel = 500, multiplier = 5 }
}

-- Rates
-- NOTE: rateExp is not used if you have enabled stages above
rateExp = 5
rateSkill = 30
rateLoot = 20
rateMagic = 30
rateSpawn = 1

-- Monster Despawn Config
-- despawnRange is the amount of floors a monster can be from its spawn position
-- despawnRadius is how many tiles away it can be from its spawn position
-- removeOnDespawn will remove the monster if true or teleport it back to its spawn position if false
-- walkToSpawnRadius is the allowed distance that the monster will stay away from spawn position when left with no targets, 0 to disable
-- monsterOverspawn can be used instead of removeOnDespawn option, this will start respawn process of the monster when it goes out of deSpawn* boundaries.
-- Setting both removeOnDespawn and monsterOverspawn to true prioritizes the latter.
deSpawnRange = 2
deSpawnRadius = 50
removeOnDespawn = true
walkToSpawnRadius = 15
monsterOverspawn = false

-- Monster Level Scaling
enableMonsterLevelScaling = true
monsterLevelRules = {
  { minZ = 0, maxZ = 0,  levelsPerFloor = 0 },
  { minZ = 1, maxZ = 1,  levelsPerFloor = 0 },
  { minZ = 2, maxZ = 2,  levelsPerFloor = 0 },
  { minZ = 3, maxZ = 3,  levelsPerFloor = 0 },
  { minZ = 4, maxZ = 4,  levelsPerFloor = 0 },
  { minZ = 5, maxZ = 5,  levelsPerFloor = 0 },
  { minZ = 6, maxZ = 6,  levelsPerFloor = 0 },
  { minZ = 7, maxZ = 7,  levelsPerFloor = 1 },
  { minZ = 8, maxZ = 8,  levelsPerFloor = 1 },
  { minZ = 9, maxZ = 9,  levelsPerFloor = 1 },
  { minZ = 10, maxZ = 10, levelsPerFloor = 1 },
  { minZ = 11, maxZ = 11, levelsPerFloor = 3 },
  { minZ = 12, maxZ = 12, levelsPerFloor = 3 },
  { minZ = 13, maxZ = 13, levelsPerFloor = 4 },
  { minZ = 14, maxZ = 14, levelsPerFloor = 4 },
  { minZ = 15, maxZ = 20, levelsPerFloor = 5 }
}
monsterBonusHealth = 0.5
monsterBonusDamage = 0.02
monsterBonusSpeed  = 0.02
monsterBonusLoot   = 0.04

-- Stamina
staminaSystem = true
timeToRegenMinuteStamina = 3 * 60
timeToRegenMinutePremiumStamina = 10 * 60

-- Scripts
warnUnsafeScripts = true
convertUnsafeScripts = true

-- Startup
-- NOTE: defaultPriority only works on Windows and sets process
-- priority, valid values are: "normal", "above-normal", "high"
defaultPriority = "high"
startupDatabaseOptimization = false

-- Status Server Information
ownerName = "Nexus Dev Team"
ownerEmail = "admin@devnexus.online"
url = "https://devnexus.online/"
location = "USA"
