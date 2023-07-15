#pragma once
#include <string>
#include "components/graphics/Color.h"

using namespace std;


struct Tile {
    wstring ch;
    Color fg_color;
    Color bg_color;
    
    bool visited;
    bool visible;
    
    int z;
    
    bool subterranean;
    bool emissive;

    bool harmful;
    
    
    
};

