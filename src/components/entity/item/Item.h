#pragma once

#include <string>
#include <vector>
#include "ItemTypes.h"
#include "components/entity/actors/player/Player.h"
#include "components/graphics/Sprite.h"

using namespace std;


struct Player;


struct Item{
    ItemType type;
    string name;
    wstring character;
    Color color;
    bool emissive;
    Sprite sprite;
    float rarity;
    function<void(Player&)> effect;
};