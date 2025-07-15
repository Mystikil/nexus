-- Example gate spawn configuration
GateSpawnConfig = {
    center = {x = 1000, y = 1000, z = 7},
    radius = 25,
    interval = 60000,
    rules = {
        {rank = GateRank.E, type = GateType.NORMAL, max = 2},
        {rank = GateRank.D, type = GateType.RED,    max = 1}
    }
}
