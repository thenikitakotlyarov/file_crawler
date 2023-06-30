#pragma once


#include "constants.h"
#include "components/Tile.h"


struct GameMap {
    string meta;
    vector<vector<Tile>> data;
};