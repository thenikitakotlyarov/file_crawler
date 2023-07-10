#pragma once
#include <string>
#include "components/graphics/Color.h"

using namespace std;


struct Tile {
    wstring ch;
    Color color;
    
    bool visited;
    bool visible;
    
    int z;
    
    bool subterranean;
    bool emissive;
    
    
    
};

