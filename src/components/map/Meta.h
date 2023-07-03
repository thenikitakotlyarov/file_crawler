#pragma once

#include "constants.h"
#include <string>
#include "components/item/Item.h"
#include "components/enemy/Monster.h"
#include "templates/item/BaseItems.h"
#include "templates/enemy/BaseEnemies.h"


struct Item;
struct Monster;


struct Meta {
    string name;
    vector<Item> itemTemplates;
    vector<Monster> monsterTemplates;
};
