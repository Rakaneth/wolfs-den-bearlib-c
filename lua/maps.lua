mapdata = {
    mine = {
        width = 85,
        height = 85,
        wall_color = "stone",
        type = "dungeon",
        name = "Mines (Upper)",
        down = "lower-mines",
        up = "world=map"
    },
    ["lower-mines"] = {
        width = 85,
        height = 85,
        wall_color = "stone",
        type = "dungeon",
        name = "Mine (Lower)",
        down = "caves",
        up = "mine"
    },
    caves = {
        width = 100,
        height = 100,
        wall_color = "sepia",
        type = "caves",
        name = "Caves (Upper)",
        down = "lower-caves",
        up = "lower-mines"
    },
    ["lower-caves"] = {
        width = 100,
        height = 100,
        wall_color = "sepia",
        type = "caves",
        name = "Caves (Lower)",
        down = "crypt",
        up = "caves"
    },
    crypt = {
        width = 150,
        height = 100,
        wall_color = "sepia",
        type = "dungeon",
        name = "Crypt",
        up = "caves"
    }
}
