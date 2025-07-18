-- shader_system.lua
-- Defines shaders and item mappings used by the shader action

SHADER_LIST = {
    red_ruby = {
        shader = 'Outfit - Rainbow',
        storage = 50000
    },
    ghost = {
        shader = 'Outfit - Ghost',
        storage = 50001
    },
    frozen = {
        shader = 'Outfit - Jelly',
        storage = 50002
    }
}

-- Items that apply shaders when used on a player
SHADER_ITEMS = {
    [2147] = { key = 'red_ruby', consume = true, toggle = true }
}

OPCODE_SHADER = 9
