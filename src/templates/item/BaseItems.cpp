# include "BaseItems.h"


Item getPotionTemplate() {
    return {
            ItemType::SmallHealthPotion, //< identifier
            L"o", "Small Health Potion", NCOLOR_RED, 0.5, //< character, name, color, rarity
            [](Player &stats) {
                stats.current_health =
                        min(stats.max_health, stats.current_health + 25);
            }
    };
}

Item getSerumTemplate() {
    return {
            ItemType::SmallEnergySerum, //< identifier
            L"o", "Small Energy Serum", NCOLOR_BLUE, 0.25, //< character, name, color, rarity
            [](Player &stats) {
                stats.current_energy =
                        min(stats.max_energy, stats.current_energy + 25);
            }
    };
}

Item getSalveTemplate() {
    return {
            ItemType::SmallStaminaSalve, //< identifier
            L"o", "Small Stamina Salve", NCOLOR_ORANGE, 0.25, //< character, name, color, rarity
            [](Player &stats) {
                stats.current_energy =
                        min(stats.max_energy, stats.current_energy + 25);
            }
    };
}

Item getVitalityOrbTemplate() {
    return {
            ItemType::VitalityOrb, //< identifier
            L"⍟", "Vitality Orb", NCOLOR_RED, 0.1, //< character, name, color, rarity
            [](Player &stats) {
                stats.vitality += 3;
            }
    };
}

Item getPowerOrbTemplate() {
    return {
            ItemType::PowerOrb, //< identifier
            L"⍟", "Power Orb", NCOLOR_ORANGE, 0.1, //< character, name, color, rarity
            [](Player &stats) {
                stats.power += 3;
            }
    };
}

Item getAgiltyOrbTemplate() {
    return {
            ItemType::AgilityOrb, //< identifier
            L"⍟", "Agility Orb", NCOLOR_YELLOW, 0.1, //< character, name, color, rarity
            [](Player &stats) {
                stats.agility += 3;
            }
    };
}

Item getFocusOrbTemplate() {
    return {
            ItemType::FocusOrb, //< identifier
            L"⍟", "Focus Orb", NCOLOR_GREEN, 0.1, //< character, name, color, rarity
            [](Player &stats) {
                stats.focus += 3;
            }
    };
}

Item getInsightOrbTemplate() {
    return {
            ItemType::InsightOrb, //< identifier
            L"⍟", "Insight Orb", NCOLOR_BLUE, 0.1, //< character, name, color, rarity
            [](Player &stats) {
                stats.insight += 3;
            }
    };
}

Item getFaithOrbTemplate() {
    return {
            ItemType::BeliefOrb, //< identifier
            L"⍟", "Belief Orb", NCOLOR_PINK, 0.1, //< character, name, color, rarity
            [](Player &stats) {
                stats.belief += 3;
            }
    };
}


Item getSpecialOrbTemplate() {
    return {
            ItemType::SpecialOrb, //< identifier
            L"⍟", "Vitality Orb", NCOLOR_RED, 0.1, //< character, name, color, rarity
            [](Player &stats) {
                if (!get_random_int(0,1)) {
                    stats.vitality += 1;
                } else stats.power += 1;
                if (!get_random_int(0,1)) {
                    stats.agility += 1;
                } else stats.focus += 1;
                if (!get_random_int(0,1)) {
                    stats.insight += 1;
                } else stats.belief += 1;
            }
    };
}


