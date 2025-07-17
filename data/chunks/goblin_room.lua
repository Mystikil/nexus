return {
    name = 'goblin_room',
    size = {x = 5, y = 5, z = 1},
    ground = 416, -- stone tile
    items = {
        {id = 1740, pos = {x = 2, y = 2, z = 0}, container = {{id = 2148, count = 10}}}
    },
    monsters = {
        {name = 'Goblin', pos = {x = 1, y = 1, z = 0}},
        {name = 'Goblin', pos = {x = 3, y = 3, z = 0}},
        {name = 'Goblin Leader', pos = {x = 2, y = 4, z = 0}}
    }
}
