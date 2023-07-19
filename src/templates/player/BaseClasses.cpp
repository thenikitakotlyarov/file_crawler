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

Sprite getStandardPlayerSprite() {
    return Sprite{
            {
                    {L" ",         L" ",         L"@",         L" ",         L" "},
                    {L" ",         L"!",         L"W",         L"!",         L" "},
                    {L" ",         L"|",         L"Y",         L"|",         L" "},
                    {L" ",         L" ",         L"Λ",         L" ",         L" "},
                    {L" ",         L"J",         L" ",         L"L",         L" "}
            },
            {
                    {NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK},
                    {NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK},
                    {NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK},
                    {NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK},
                    {NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK}

            },
            {
                    {NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_LGREY, NCOLOR_BLACK, NCOLOR_BLACK},
                    {NCOLOR_BLACK, NCOLOR_MGREY, NCOLOR_MGREY, NCOLOR_MGREY, NCOLOR_BLACK},
                    {NCOLOR_BLACK, NCOLOR_DGREY, NCOLOR_MGREY, NCOLOR_DGREY, NCOLOR_BLACK},
                    {NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_MGREY, NCOLOR_BLACK, NCOLOR_BLACK},
                    {NCOLOR_BLACK, NCOLOR_DGREY, NCOLOR_BLACK, NCOLOR_DGREY, NCOLOR_BLACK}

            }
    };

}

Player getAdventurerTemplate() {
    return {
            "Link", "Adventurer", NCOLOR_LGREY,
            getStandardPlayerSprite(),
            1, 0,
            100, 100, 100,
            100, 100, 100,
            10, 10, 10, 10, 10, 10,

            getBasicAttack(),
            getAVGBoostedAttack()
    };
}


Player getBarbarianTemplate() {
    return {
            "Conan", "Barbarian", NCOLOR_RED,
            getGarb(getStandardPlayerSprite(),NCOLOR_RED),
            1, 0,
            100, 100, 100,
            100, 100, 100,
            15, 12, 7, 7, 7, 12,

            getBasicAttack(),
            getVITBoostedAttack()
    };

}


Player getDuelistTemplate() {
    return {
            "Geralt", "Duelist", NCOLOR_ORANGE,
            getGarb(getStandardPlayerSprite(),NCOLOR_ORANGE),
            1, 0,
            100, 100, 100,
            100, 100, 100,
            13, 13, 10, 7, 7, 10,

            getBasicAttack(),
            getPOWVITBoostedAttack()
    };
}


Player getFighterTemplate() {
    return {
            "Ryu", "Fighter", NCOLOR_YELLOW,
            getGarb(getStandardPlayerSprite(),NCOLOR_YELLOW),
            1, 0,
            100, 100, 100,
            100, 100, 100,
            12, 15, 12, 7, 7, 7,

            getBasicAttack(),
            getPOWBoostedAttack()
    };
}


Player getRogueTemplate() {
    return {
            "Garrett", "Rogue", NCOLOR_LGREEN,
            getGarb(getStandardPlayerSprite(),NCOLOR_LGREEN),
            1, 0,
            100, 100, 100,
            100, 100, 100,
            10, 13, 13, 10, 7, 7,

            getBasicAttack(),
            getAGIPOWBoostedAttack()
    };
}

Player getRangerTemplate() {
    return {
            "Artemis", "Ranger", NCOLOR_GREEN,
            getGarb(getStandardPlayerSprite(),NCOLOR_GREEN),
            1, 0,
            100, 100, 100,
            100, 100, 100,
            7, 12, 15, 12, 7, 7,

            getBasicAttack(),
            getAGIBoostedAttack()
    };
}


Player getDruidTemplate() {
    return {
            "Malfurion", "Druid", NCOLOR_CYAN,
            getGarb(getStandardPlayerSprite(),NCOLOR_CYAN),
            1, 0,
            100, 100, 100,
            100, 100, 100,
            7, 10, 13, 13, 10, 7,

            getBasicAttack(),
            getFOCAGIBoostedAttack()
    };
}


Player getSorcererTemplate() {
    return {
            "Gandalf", "Sorcerer", NCOLOR_AMARINE,
            getGarb(getStandardPlayerSprite(),NCOLOR_AMARINE),
            1, 0,
            100, 100, 100,
            100, 100, 100,
            7, 7, 12, 15, 12, 7,

            getBasicAttack(),
            getFOCBoostedAttack()
    };
}


Player getBardTemplate() {
    return {
            "Garrick", "Bard", NCOLOR_LBLUE,
            getGarb(getStandardPlayerSprite(),NCOLOR_LBLUE),
            1, 0,
            100, 100, 100,
            100, 100, 100,
            7, 7, 10, 13, 13, 10,

            getBasicAttack(),
            getINSFOCBoostedAttack()
    };
}


Player getWizardTemplate() {
    return {
            "Merlin", "Wizard", NCOLOR_BLUE,
            getGarb(getStandardPlayerSprite(),NCOLOR_BLUE),
            1, 0,
            100, 100, 100,
            100, 100, 100,
            7, 7, 7, 12, 15, 12,

            getBasicAttack(),
            getINSBoostedAttack()
    };
}

Player getMonkTemplate() {
    return {
            "Sabin", "Monk", NCOLOR_PURPLE,
            getGarb(getStandardPlayerSprite(),NCOLOR_PURPLE),
            1, 0,
            100, 100, 100,
            100, 100, 100,
            10, 7, 7, 10, 13, 13,

            getBasicAttack(),
            getBELINSBoostedAttack()
    };
}


Player getClericTemplate() {
    return {
            "Alistair", "Cleric", NCOLOR_PINK,
            getGarb(getStandardPlayerSprite(),NCOLOR_PINK),
            1, 0,
            100, 100, 100,
            100, 100, 100,
            12, 7, 7, 7, 12, 15,

            getBasicAttack(),
            getBELBoostedAttack()
    };
}


Player getPaladinTemplate() {
    return {
            "Tyrael", "Paladin", NCOLOR_MAGENTA,
            getGarb(getStandardPlayerSprite(),NCOLOR_MAGENTA),
            1, 0,
            100, 100, 100,
            100, 100, 100,
            12, 10, 7, 7, 10, 13,

            getBasicAttack(),
            getVITBELBoostedAttack()
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

