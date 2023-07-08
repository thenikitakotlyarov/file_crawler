#pragma once

#include "constants.h"
#include <string>
#include "components/entity/item/Item.h"
#include "components/entity/actors/enemy/Monster.h"
#include "templates/item/BaseItems.h"
#include "templates/enemy/BaseEnemies.h"


struct Item;
struct Monster;


struct MapMeta {
    string name;
    vector<Item> itemTemplates;
    vector<Monster> monsterTemplates;
};
