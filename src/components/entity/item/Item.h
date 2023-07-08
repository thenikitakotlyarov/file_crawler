#pragma once

#include <string>
#include <vector>
#include "ItemTypes.h"
#include "components/entity/actors/player/Player.h"

using namespace std;


struct Player;


struct Item{
    ItemType type;
    wstring character;
    string name;
    Color color;
    float rarity;
    function<void(Player&)> effect;
};