#pragma once


#include "constants.h"
#include "Tile.h"
#include "MapMeta.h"


struct GameMap {
    MapMeta meta;
    vector<vector<Tile>> data;
};