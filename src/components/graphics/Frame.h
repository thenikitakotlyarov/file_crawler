#pragma once

#include <string>
#include <vector>
#include "Color.h"
#include "Pixel.h"

using namespace std;

struct FrameMeta {
    unsigned long id;
    string name;
    string comments;
};

struct Frame {
    FrameMeta meta;
    vector<vector<Pixel>> data;
};