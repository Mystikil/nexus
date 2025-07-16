# Combat Hooks

The engine emits two hooks around damage calculations. Lua scripts can
register callbacks using `EventManager.register`.

## Available Hooks

- `combat:preDamage`
- `combat:postDamage`

Callbacks receive the following parameters:

```lua
function callback(caster, target, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)
    -- caster may be nil
end
```

`primaryDamage` and `secondaryDamage` correspond to the damage values
before sign inversion. Types are combat types and `origin` matches the
`CombatOrigin` enum.
