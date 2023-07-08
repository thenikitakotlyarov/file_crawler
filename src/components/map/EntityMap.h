#pragma once


#include "constants.h"
#include "components/entity/Entity.h"


struct EntityMap {
    MapMeta meta;
    vector<vector<vector<Entity>>> data;
};