# include "BaseItems.h"

const vector<vector<wstring>> potion_texture = {
        {L"╭", L"⛁", L"╮"},
        {L"│", L"▒", L"│"},
        {L"╰", L"─", L"╯"}

};

const vector<vector<wstring>> orb_texture = {
        {L"╭", L"╥", L"╮"},
        {L"╞", L"🏶", L"╡"},
        {L"╰", L"╨", L"╯"}
};


Item getNullItem() {
    return Item{
            ItemType::NullItem,
            {},
            {},
            {},
            {},
            {},
            {},
            {}

    };
}

Item getSmallPotionTemplate() {
    return {
            ItemType::SmallHealthPotion, //< identifier
            L"Potion", L"o", NCOLOR_RED, false,
            {potion_texture,
             {
                     {NCOLOR_LGREY, NCOLOR_LYELLOW, NCOLOR_WHITE},
                     {NCOLOR_MGREY, NCOLOR_LRED, NCOLOR_LGREY},
                     {NCOLOR_DGREY, NCOLOR_DGREY, NCOLOR_MGREY}
             },
             {
                     {NCOLOR_MRED, NCOLOR_DYELLOW, NCOLOR_LRED},
                     {NCOLOR_DRED, NCOLOR_MRED, NCOLOR_MRED},
                     {NCOLOR_BLACK, NCOLOR_DRED, NCOLOR_MRED}
             }},
            0.5,
            [](Player &stats) {
                stats.health.first =
                        min(stats.health.second, stats.health.first + 25);
            }
    };
}

Item getSmallSerumTemplate() {
    return {
            ItemType::SmallEnergyPotion, //< identifier
            L"Serum", L"o", NCOLOR_BLUE, false,
            {potion_texture,
             {
                     {NCOLOR_LGREY, NCOLOR_LYELLOW, NCOLOR_WHITE},
                     {NCOLOR_MGREY, NCOLOR_LBLUE, NCOLOR_LGREY},
                     {NCOLOR_DGREY, NCOLOR_DGREY, NCOLOR_MGREY}
             },
             {
                     {NCOLOR_MBLUE, NCOLOR_DYELLOW, NCOLOR_LBLUE},
                     {NCOLOR_DBLUE, NCOLOR_MBLUE, NCOLOR_MBLUE},
                     {NCOLOR_BLACK, NCOLOR_DBLUE, NCOLOR_MBLUE}
             }},
            0.25,
            [](Player &stats) {
                stats.energy.first =
                        min(stats.energy.second, stats.energy.first + 25);
            }
    };
}

Item getSmallSalveTemplate() {
    return {
            ItemType::SmallStaminaPotion, //< identifier
            L"Salve", L"o", NCOLOR_GREEN, false,
            {potion_texture,
             {
                     {NCOLOR_LGREY, NCOLOR_LYELLOW, NCOLOR_WHITE},
                     {NCOLOR_MGREY, NCOLOR_LGREEN, NCOLOR_LGREY},
                     {NCOLOR_DGREY, NCOLOR_DGREY, NCOLOR_MGREY}
             },
             {
                     {NCOLOR_MGREEN, NCOLOR_DYELLOW, NCOLOR_LGREEN},
                     {NCOLOR_DGREEN, NCOLOR_MGREEN, NCOLOR_MGREEN},
                     {NCOLOR_BLACK, NCOLOR_DGREEN, NCOLOR_MGREEN}
             }},
            0.25,
            [](Player &stats) {
                stats.stamina.first =
                        min(stats.stamina.second, stats.stamina.first + 25);
            }
    };
}

Item getVitalityOrbTemplate() {
    return {
            ItemType::VitalityOrb, //< identifier
            L"Vitality Orb", L"⍟", NCOLOR_RED, true,
            {orb_texture,
             {
                     {NCOLOR_LGREY, NCOLOR_LGREY, NCOLOR_WHITE},
                     {NCOLOR_MGREY, NCOLOR_RED, NCOLOR_LGREY},
                     {NCOLOR_DGREY, NCOLOR_DGREY, NCOLOR_MGREY}
             },
             {
                     {NCOLOR_MRED, NCOLOR_MRED, NCOLOR_LRED},
                     {NCOLOR_DRED, NCOLOR_DRED, NCOLOR_MRED},
                     {NCOLOR_BLACK, NCOLOR_DRED, NCOLOR_MRED}
             }},
            0.1,
            [](Player &stats) {
                stats.vitality += 1;
            }
    };
}

Item getPowerOrbTemplate() {
    return {
            ItemType::PowerOrb, //< identifier
            L"POW Orb", L"⍟", NCOLOR_YELLOW, true,
            {orb_texture,
             {
                     {NCOLOR_LGREY, NCOLOR_LGREY, NCOLOR_WHITE},
                     {NCOLOR_MGREY, NCOLOR_YELLOW, NCOLOR_LGREY},
                     {NCOLOR_DGREY, NCOLOR_DGREY, NCOLOR_MGREY}
             },
             {
                     {NCOLOR_MYELLOW, NCOLOR_MYELLOW, NCOLOR_LYELLOW},
                     {NCOLOR_DYELLOW, NCOLOR_DYELLOW, NCOLOR_MYELLOW},
                     {NCOLOR_BLACK, NCOLOR_DYELLOW, NCOLOR_MYELLOW}
             }},
            0.1,
            [](Player &stats) {
                stats.power += 1;
            }
    };
}

Item getAgiltyOrbTemplate() {
    return {
            ItemType::AgilityOrb, //< identifier
            L"AGI Orb", L"⍟", NCOLOR_GREEN, true,
            {orb_texture,
             {
                     {NCOLOR_LGREY, NCOLOR_LGREY, NCOLOR_WHITE},
                     {NCOLOR_MGREY, NCOLOR_GREEN, NCOLOR_LGREY},
                     {NCOLOR_DGREY, NCOLOR_DGREY, NCOLOR_MGREY}
             },
             {
                     {NCOLOR_MGREEN, NCOLOR_MGREEN, NCOLOR_LGREEN},
                     {NCOLOR_DGREEN, NCOLOR_DGREEN, NCOLOR_MGREEN},
                     {NCOLOR_BLACK, NCOLOR_DGREEN, NCOLOR_MGREEN}
             }},
            0.1,
            [](Player &stats) {
                stats.agility += 1;
            }
    };
}

Item getFocusOrbTemplate() {
    return {
            ItemType::FocusOrb, //< identifier
            L"FOC Orb", L"⍟", NCOLOR_CYAN, true,
            {orb_texture,
             {
                     {NCOLOR_LGREY, NCOLOR_LGREY, NCOLOR_WHITE},
                     {NCOLOR_MGREY, NCOLOR_CYAN, NCOLOR_LGREY},
                     {NCOLOR_DGREY, NCOLOR_DGREY, NCOLOR_MGREY}
             },
             {
                     {NCOLOR_MCYAN, NCOLOR_MCYAN, NCOLOR_LCYAN},
                     {NCOLOR_DCYAN, NCOLOR_DCYAN, NCOLOR_MCYAN},
                     {NCOLOR_BLACK, NCOLOR_DCYAN, NCOLOR_MCYAN}
             }},
            0.1,
            [](Player &stats) {
                stats.focus += 1;
            }
    };
}

Item getInsightOrbTemplate() {
    return {
            ItemType::InsightOrb, //< identifier
            L"INS Orb", L"⍟", NCOLOR_BLUE, true,
            {orb_texture,
             {
                     {NCOLOR_LGREY, NCOLOR_LGREY, NCOLOR_WHITE},
                     {NCOLOR_MGREY, NCOLOR_BLUE, NCOLOR_LGREY},
                     {NCOLOR_DGREY, NCOLOR_DGREY, NCOLOR_MGREY}
             },
             {
                     {NCOLOR_MBLUE, NCOLOR_MBLUE, NCOLOR_LBLUE},
                     {NCOLOR_DBLUE, NCOLOR_DBLUE, NCOLOR_MBLUE},
                     {NCOLOR_BLACK, NCOLOR_DBLUE, NCOLOR_MBLUE}
             }},
            0.1,
            [](Player &stats) {
                stats.insight += 1;
            }
    };
}

Item getFaithOrbTemplate() {
    return {
            ItemType::BeliefOrb, //< identifier
            L"BEL Orb", L"⍟", NCOLOR_MAGENTA, true,
            {orb_texture,
             {
                     {NCOLOR_LGREY, NCOLOR_LGREY, NCOLOR_WHITE},
                     {NCOLOR_MGREY, NCOLOR_MAGENTA, NCOLOR_LGREY},
                     {NCOLOR_DGREY, NCOLOR_DGREY, NCOLOR_MGREY}
             },
             {
                     {NCOLOR_MMAGENTA, NCOLOR_MMAGENTA, NCOLOR_LMAGENTA},
                     {NCOLOR_DMAGENTA, NCOLOR_DMAGENTA, NCOLOR_MMAGENTA},
                     {NCOLOR_BLACK, NCOLOR_DMAGENTA, NCOLOR_MMAGENTA}
             }},
            0.1,
            [](Player &stats) {
                stats.belief += 1;
            }
    };
}


Item getSpecialOrbTemplate() {
    return {
            ItemType::SpecialOrb, //< identifier
            L"??? Orb", L"⍟", NCOLOR_LGREY, true,
            {orb_texture,
             {
                     {NCOLOR_LGREY, NCOLOR_LGREY, NCOLOR_WHITE},
                     {NCOLOR_MGREY, NCOLOR_WHITE, NCOLOR_LGREY},
                     {NCOLOR_DGREY, NCOLOR_DGREY, NCOLOR_MGREY}
             },
             {
                     {NCOLOR_MGREY, NCOLOR_MGREY, NCOLOR_LGREY},
                     {NCOLOR_DGREY, NCOLOR_DGREY, NCOLOR_MGREY},
                     {NCOLOR_BLACK, NCOLOR_DGREY, NCOLOR_MGREY}
             }},
            0.1,
            [](Player &stats) {
                if (!get_random_int(0, 1)) {
                    stats.vitality += 1;
                } else stats.power += 1;
                if (!get_random_int(0, 1)) {
                    stats.agility += 1;
                } else stats.focus += 1;
                if (!get_random_int(0, 1)) {
                    stats.insight += 1;
                } else stats.belief += 1;
            }
    };
}


