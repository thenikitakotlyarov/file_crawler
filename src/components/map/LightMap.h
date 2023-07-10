#pragma once

#include "components/graphics/Light.h"
using namespace std;


struct LightMap {
    MapMeta meta;
    vector<vector<Light>> data;
};