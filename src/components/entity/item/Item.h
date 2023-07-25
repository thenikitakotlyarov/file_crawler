#pragma once

#include <string>
#include <vector>
#include "components/entity/item/ItemTypes.h"
#include "components/entity/actors/player/Player.h"
#include "components/graphics/Sprite.h"

using namespace std;


struct Player;


struct Item{
    ItemType type;
    wstring name;
    wstring character;
    Color color;
    bool emissive;
    Sprite sprite;
    float rarity;
    function<void(Player&)> effect;

    Item& operator=(const Item& other) {
        if (this != &other) {
            // your assignment logic here
            // note: you can't reseat the 'item' reference to refer to a different object
            type = other.type;
            name = other.name;
            character = other.character;
            color = other.color;
            emissive = other.emissive;
            sprite = other.sprite;
            rarity = other.rarity;
            effect = other.effect;
        }
        return *this;

    }
};
