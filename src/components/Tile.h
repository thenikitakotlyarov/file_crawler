#pragma once
#include <string>

using namespace std;

struct Tile {
    wstring ch;
    short color;
    bool visited;
    bool visible;
    int z;
};