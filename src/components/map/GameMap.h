#pragma once


#include "constants.h"
#include "components/Tile.h"
#include "Meta.h"


struct GameMap {
    Meta meta;
    vector<vector<Tile>> data;
};