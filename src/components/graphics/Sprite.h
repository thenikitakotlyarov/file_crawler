#pragma once

#include "helpers.h"
using namespace std;

struct Sprite {
    vector<vector<wstring>> texture;
    vector<vector<Color>> fg_colors;
    vector<vector<Color>> bg_colors;
};