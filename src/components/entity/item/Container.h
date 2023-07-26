#pragma once


using namespace std;

#include "components/entity/item/Item.h"

struct Item;

struct ContainerSlot {
    Item *item;
    unsigned short size;
    unsigned short quantity;
};


struct Container {
    unsigned short id;
    vector<ContainerSlot> content;
};