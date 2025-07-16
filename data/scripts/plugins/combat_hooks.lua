EventManager.register("combat:preDamage", function(caster, target, pDamage, pType, sDamage, sType, origin)
    print(string.format("[Lua] preDamage %s -> %s %d", caster and caster:getName() or 'nil', target:getName(), pDamage))
end)

EventManager.register("combat:postDamage", function(caster, target, pDamage, pType, sDamage, sType, origin)
    print(string.format("[Lua] postDamage dealt %d", pDamage))
end)
