#pragma once


#include "constants.h"
#include "components/Entity.h"


using namespace std;

struct EntityMap {
    string meta;
    vector<vector<vector<Entity>>> data;
};