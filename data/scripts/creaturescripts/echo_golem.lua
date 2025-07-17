-- Monster specific wrapper delegating to the shared EchoAI
local EchoAI = dofile('data/scripts/monsters/echo_base.lua')
local echoEvent = CreatureEvent('EchoGolem')

echoEvent.onSpawn = function(monster, position, startup, artificial)
    return EchoAI.onSpawn(monster, position, startup, artificial)
end

echoEvent.onThink = function(monster, interval)
    return EchoAI.onThink(monster, interval)
end

echoEvent.onHealthChange = function(monster, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)
    return EchoAI.onHealthChange(monster, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)
end

echoEvent.onDeath = function(monster, corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified)
    return EchoAI.onDeath(monster, corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified)
end

echoEvent:register()
