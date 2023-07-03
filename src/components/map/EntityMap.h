#pragma once


#include "constants.h"
#include "components/Entity.h"


struct EntityMap {
    Meta meta;
    vector<vector<vector<Entity>>> data;
};