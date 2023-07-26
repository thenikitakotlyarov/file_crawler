#pragma once

//#include "templates/item/BaseItems.h"


enum class ItemType {//TODO:figure out a way to load these automatically
    NullItem,
    SmallHealthPotion,
    SmallEnergyPotion,
    SmallStaminaPotion,

    VitalityOrb,
    PowerOrb,
    AgilityOrb,
    FocusOrb,
    InsightOrb,
    BeliefOrb,
    SpecialOrb
};

namespace std {
    template <> struct hash<ItemType> {
        size_t operator()(const ItemType &t) const {
            return static_cast<size_t>(t);
        }
    };
}


const unordered_set<ItemType,hash<ItemType>> PotionType {
        ItemType::SmallHealthPotion,
        ItemType::SmallEnergyPotion,
        ItemType::SmallStaminaPotion
};

const unordered_set<ItemType,hash<ItemType>> OrbType {
    ItemType::VitalityOrb,
    ItemType::PowerOrb,
    ItemType::AgilityOrb,
    ItemType::FocusOrb,
    ItemType::InsightOrb,
    ItemType::BeliefOrb,
    ItemType::SpecialOrb
};