#include "BaseClasses.h"

Color getGarbColor(pair<Color, Color> compare) {
    Color this_color = NCOLOR_BLACK;
    if (compare.first == NCOLOR_RED) {
        if (compare.second == NCOLOR_LGREY) { this_color = NCOLOR_LRED; }
        else if (compare.second == NCOLOR_MGREY) { this_color = NCOLOR_MRED; }
        else if (compare.second == NCOLOR_DGREY) { this_color = NCOLOR_DRED; }
    } else if (compare.first == NCOLOR_ORANGE) {
        if (compare.second == NCOLOR_LGREY) { this_color = NCOLOR_LYELLOW; }
        else if (compare.second == NCOLOR_MGREY) { this_color = NCOLOR_ORANGE; }
        else if (compare.second == NCOLOR_DGREY) { this_color = NCOLOR_DRED; }
    } else if (compare.first == NCOLOR_YELLOW) {
        if (compare.second == NCOLOR_LGREY) { this_color = NCOLOR_LYELLOW; }
        else if (compare.second == NCOLOR_MGREY) { this_color = NCOLOR_MYELLOW; }
        else if (compare.second == NCOLOR_DGREY) { this_color = NCOLOR_DYELLOW; }
    } else if (compare.first == NCOLOR_LGREEN) {
        if (compare.second == NCOLOR_LGREY) { this_color = NCOLOR_LCYAN; }
        else if (compare.second == NCOLOR_MGREY) { this_color = NCOLOR_LGREEN; }
        else if (compare.second == NCOLOR_DGREY) { this_color = NCOLOR_DYELLOW; }
    } else if (compare.first == NCOLOR_GREEN) {
        if (compare.second == NCOLOR_LGREY) { this_color = NCOLOR_LGREEN; }
        else if (compare.second == NCOLOR_MGREY) { this_color = NCOLOR_MGREEN; }
        else if (compare.second == NCOLOR_DGREY) { this_color = NCOLOR_DGREEN; }
    } else if (compare.first == NCOLOR_CYAN) {
        if (compare.second == NCOLOR_LGREY) { this_color = NCOLOR_LCYAN; }
        else if (compare.second == NCOLOR_MGREY) { this_color = NCOLOR_MCYAN; }
        else if (compare.second == NCOLOR_DGREY) { this_color = NCOLOR_DCYAN; }
    } else if (compare.first == NCOLOR_AMARINE) {
        if (compare.second == NCOLOR_LGREY) { this_color = NCOLOR_LBLUE; }
        else if (compare.second == NCOLOR_MGREY) { this_color = NCOLOR_AMARINE; }
        else if (compare.second == NCOLOR_DGREY) { this_color = NCOLOR_DCYAN; }
    } else if (compare.first == NCOLOR_LBLUE) {
        if (compare.second == NCOLOR_LGREY) { this_color = NCOLOR_LCYAN; }
        else if (compare.second == NCOLOR_MGREY) { this_color = NCOLOR_LBLUE; }
        else if (compare.second == NCOLOR_DGREY) { this_color = NCOLOR_DBLUE; }
    } else if (compare.first == NCOLOR_BLUE) {
        if (compare.second == NCOLOR_LGREY) { this_color = NCOLOR_LBLUE; }
        else if (compare.second == NCOLOR_MGREY) { this_color = NCOLOR_MBLUE; }
        else if (compare.second == NCOLOR_DGREY) { this_color = NCOLOR_DBLUE; }
    } else if (compare.first == NCOLOR_PURPLE) {
        if (compare.second == NCOLOR_LGREY) { this_color = NCOLOR_LMAGENTA; }
        else if (compare.second == NCOLOR_MGREY) { this_color = NCOLOR_PURPLE; }
        else if (compare.second == NCOLOR_DGREY) { this_color = NCOLOR_DBLUE; }
    } else if (compare.first == NCOLOR_PINK) {
        if (compare.second == NCOLOR_LGREY) { this_color = NCOLOR_LMAGENTA; }
        else if (compare.second == NCOLOR_MGREY) { this_color = NCOLOR_PINK; }
        else if (compare.second == NCOLOR_DGREY) { this_color = NCOLOR_DBLUE; }
    } else if (compare.first == NCOLOR_MAGENTA) {
        if (compare.second == NCOLOR_LGREY) { this_color = NCOLOR_LMAGENTA; }
        else if (compare.second == NCOLOR_MGREY) { this_color = NCOLOR_MMAGENTA; }
        else if (compare.second == NCOLOR_DGREY) { this_color = NCOLOR_DMAGENTA; }
    }

    return this_color;
}

Sprite getGarb(Sprite player_sprite, const Color player_color) {
    for (int i = 0; i < player_sprite.texture.size(); ++i) {
        for (int j = 0; j < player_sprite.texture[0].size(); ++j) {
            player_sprite.bg_colors[i][j] = getGarbColor(
                    make_pair(player_color, player_sprite.bg_colors[i][j]));
        }
    }
    return player_sprite;
}

Sprite getStandardPlayerSprite(const Color garb) {
    Sprite this_sprite = {
            {
                    {L"?",         L"@",         L"?"},
                    {L"!",         L"W",         L"!"},
                    {L"|",         L"Y",         L"|"},
                    {L"?",         L"Λ",         L"?"},
                    {L"J",         L"?",         L"L"}
            },
            {
                    {NCOLOR_RED,   NCOLOR_BLACK, NCOLOR_RED},
                    {NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK},
                    {NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK},
                    {NCOLOR_RED,   NCOLOR_BLACK, NCOLOR_RED},
                    {NCOLOR_BLACK, NCOLOR_RED,   NCOLOR_BLACK}

            },
            {
                    {NCOLOR_BLACK, NCOLOR_LGREY, NCOLOR_BLACK},
                    {NCOLOR_MGREY, NCOLOR_MGREY, NCOLOR_MGREY},
                    {NCOLOR_DGREY, NCOLOR_MGREY, NCOLOR_DGREY},
                    {NCOLOR_BLACK, NCOLOR_MGREY, NCOLOR_BLACK},
                    {NCOLOR_DGREY, NCOLOR_BLACK, NCOLOR_DGREY}
            }
    };

    if (garb != NCOLOR_LGREY) {
        this_sprite = getGarb(this_sprite,garb);
    }

    return this_sprite;

}

Container getEmptyInventory(const int size) {
    ContainerSlot slot = {new Item(getNullItem()), 3, 0};
    return Container{0, vector<ContainerSlot>(size, slot)};
}

Player getAdventurerTemplate() {
    return {
            L"Link", L"Adventurer", NCOLOR_LGREY,
            getStandardPlayerSprite(NCOLOR_LGREY),
            1,
            {100, 100}, {100, 100}, {100, 100},
            10, 10, 10, 10, 10, 10,
            13,

            getBasicAttack(),
            getAVGBoostedAttack(),
            getEmptyInventory(3)
    };
}


Player getBarbarianTemplate() {
    return {
            L"Conan", L"Barbarian", NCOLOR_RED,
            getStandardPlayerSprite(NCOLOR_RED),
            1,
            {130, 130}, {85, 85}, {85, 85},
            15, 12, 7, 7, 7, 12,
            10,

            getBasicAttack(),
            getVITBoostedAttack(),
            getEmptyInventory(3)
    };

}


Player getDuelistTemplate() {
    return {
            L"Geralt", L"Duelist", NCOLOR_ORANGE,
            getStandardPlayerSprite(NCOLOR_ORANGE),
            1,
            {120, 120}, {100, 100}, {80, 80},
            13, 13, 10, 7, 7, 10,
            10,

            getBasicAttack(),
            getPOWVITBoostedAttack(),
            getEmptyInventory(3)
    };
}


Player getFighterTemplate() {
    return {
            L"Ryu", L"Fighter", NCOLOR_YELLOW,
            getStandardPlayerSprite(NCOLOR_YELLOW),
            1,
            {115, 115}, {115, 115}, {70, 70},
            12, 15, 12, 7, 7, 7,
            10,

            getBasicAttack(),
            getPOWBoostedAttack(),
            getEmptyInventory(3)
    };
}


Player getRogueTemplate() {
    return {
            L"Garrett", L"Rogue", NCOLOR_LGREEN,
            getStandardPlayerSprite(NCOLOR_LGREEN),
            1,
            {100, 100}, {120, 120}, {80, 80},
            10, 13, 13, 10, 7, 7,
            10,

            getBasicAttack(),
            getAGIPOWBoostedAttack(),
            getEmptyInventory(3)
    };
}

Player getRangerTemplate() {
    return {
            L"Artemis", L"Ranger", NCOLOR_GREEN,
            getStandardPlayerSprite(NCOLOR_GREEN),
            1,
            {85, 85}, {130, 130}, {85, 85},
            7, 12, 15, 12, 7, 7,
            10,

            getBasicAttack(),
            getAGIBoostedAttack(),
            getEmptyInventory(3)
    };
}


Player getDruidTemplate() {
    return {
            L"Malfurion", L"Druid", NCOLOR_CYAN,
            getStandardPlayerSprite(NCOLOR_CYAN),
            1,
            {80, 80}, {120, 120}, {100, 100},
            7, 10, 13, 13, 10, 7,
            10,

            getBasicAttack(),
            getFOCAGIBoostedAttack(),
            getEmptyInventory(3)
    };
}


Player getSorcererTemplate() {
    return {
            L"Gandalf", L"Sorcerer", NCOLOR_AMARINE,
            getStandardPlayerSprite(NCOLOR_AMARINE),
            1,
            {70, 70}, {115, 115}, {115, 115},
            7, 7, 12, 15, 12, 7,
            10,

            getBasicAttack(),
            getFOCBoostedAttack(),
            getEmptyInventory(3)
    };
}


Player getBardTemplate() {
    return {
            L"Garrick", L"Bard", NCOLOR_LBLUE,
            getStandardPlayerSprite(NCOLOR_LBLUE),
            1,
            {80, 80}, {100, 100}, {120, 120},
            7, 7, 10, 13, 13, 10,
            10,

            getBasicAttack(),
            getINSFOCBoostedAttack(),
            getEmptyInventory(3)
    };
}


Player getWizardTemplate() {
    return {
            L"Merlin", L"Wizard", NCOLOR_BLUE,
            getStandardPlayerSprite(NCOLOR_BLUE),
            1,
            {85, 85}, {85, 85}, {135, 135},
            7, 7, 7, 12, 15, 12,
            10,

            getBasicAttack(),
            getINSBoostedAttack(),
            getEmptyInventory(3)
    };
}

Player getMonkTemplate() {
    return {
            L"Sabin", L"Monk", NCOLOR_PURPLE,
            getStandardPlayerSprite(NCOLOR_PURPLE),
            1,
            {100, 100}, {80, 80}, {120, 120},
            10, 7, 7, 10, 13, 13,
            10,

            getBasicAttack(),
            getBELINSBoostedAttack(),
            getEmptyInventory(3)
    };
}


Player getClericTemplate() {
    return {
            L"Alistair", L"Cleric", NCOLOR_PINK,
            getStandardPlayerSprite(NCOLOR_PINK),
            1,
            {115, 115}, {70, 70}, {115, 115},
            12, 7, 7, 7, 12, 15,
            10,

            getBasicAttack(),
            getBELBoostedAttack(),
            getEmptyInventory(3)
    };
}


Player getPaladinTemplate() {
    return {
            L"Tyrael", L"Paladin", NCOLOR_MAGENTA,
            getStandardPlayerSprite(NCOLOR_MAGENTA),
            1,
            {120, 120}, {80, 80}, {100, 100},
            13, 10, 7, 7, 10, 13,
            10,

            getBasicAttack(),
            getVITBELBoostedAttack(),
            getEmptyInventory(3)
    };
}


Player getRandomTemplate() {
    int choice = get_random_int(0, 11);
    Player Random;
    switch (choice) {
        case (0):
            Random = getBarbarianTemplate();
            break;
        case (1):
            Random = getDuelistTemplate();
            break;
        case (2):
            Random = getFighterTemplate();
            break;
        case (3):
            Random = getRogueTemplate();
            break;
        case (4):
            Random = getRangerTemplate();
            break;
        case (5):
            Random = getDruidTemplate();
            break;
        case (6):
            Random = getSorcererTemplate();
            break;
        case (7):
            Random = getBardTemplate();
            break;
        case (8):
            Random = getWizardTemplate();
            break;
        case (9):
            Random = getMonkTemplate();
            break;
        case (10):
            Random = getClericTemplate();
            break;
        case (11):
            Random = getPaladinTemplate();
            break;
    }

    return Random;
}

