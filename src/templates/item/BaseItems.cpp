# include "BaseItems.h"


Item getPotionTemplate() {
    return {
            ItemType::SmallHealthPotion, //< identifier
            L"o", "Small Health Potion", COLOR_RED, 0.5, //< character, name, color, rarity
            [](Player &stats) {
                stats.current_health =
                        min(stats.max_health, stats.current_health + 5);
            }
    };
};

Item getSerumTemplate() {

};

Item getSalveTemplate() {

};

Item getConstitutionOrbTemplate() {

};

Item getStrengthOrbTemplate() {

};

Item getEnduranceOrbTemplate() {

};

Item getDexterityOrbTemplate() {

};

Item getIntelligenceOrbTemplate() {

};

Item getWillpowerOrbTemplate() {

};
