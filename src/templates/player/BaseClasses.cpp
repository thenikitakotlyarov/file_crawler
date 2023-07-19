#include "BaseClasses.h"


Sprite standard_player_sprite = {
        {
                {L" ", L" ", L"@", L" ", L" "},
                {L" ", L"!", L"W", L"!", L" "},
                {L" ", L"|", L"Y", L"|", L" "},
                {L" ", L" ", L"Λ", L" ", L" "},
                {L" ", L"J", L" ", L"L", L" "}
        },
        {
                {NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK},
                {NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK},
                {NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK},
                {NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK},
                {NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_BLACK}

        },
        {
                {NCOLOR_MGREY, NCOLOR_MGREY, NCOLOR_MGREY, NCOLOR_MGREY, NCOLOR_MGREY},
                {NCOLOR_MGREY, NCOLOR_MGREY, NCOLOR_MGREY, NCOLOR_MGREY, NCOLOR_MGREY},
                {NCOLOR_MGREY, NCOLOR_MGREY, NCOLOR_MGREY, NCOLOR_MGREY, NCOLOR_MGREY},
                {NCOLOR_MGREY, NCOLOR_MGREY, NCOLOR_MGREY, NCOLOR_MGREY, NCOLOR_MGREY},
                {NCOLOR_MGREY, NCOLOR_MGREY, NCOLOR_MGREY, NCOLOR_MGREY, NCOLOR_MGREY}

        }

};
Player getAdventurerTemplate() {
    return {
            "Beb", "Adventurer", NCOLOR_MGREY,
            standard_player_sprite, 1, 0,
            100, 100, 100,
            100, 100, 100,
            10, 10, 10, 10, 10, 10,

            getBasicAttack(),
            getAVGBoostedAttack()
    };
}


Player getBarbarianTemplate() {
    return {
            "", "Barbarian", NCOLOR_RED,
            standard_player_sprite, 1, 0,
            100, 100, 100,
            100, 100, 100,
            15, 12, 7, 7, 7, 12,

            getBasicAttack(),
            getVITBoostedAttack()
    };

}


Player getDuelistTemplate() {
    return {
            "", "Duelist", NCOLOR_ORANGE,
            standard_player_sprite, 1, 0,
            100, 100, 100,
            100, 100, 100,
            13, 13, 10, 7, 7, 10,

            getBasicAttack(),
            getPOWVITBoostedAttack()
    };
}


Player getFighterTemplate() {
    return {
            "", "Fighter", NCOLOR_YELLOW,
            standard_player_sprite, 1, 0,
            100, 100, 100,
            100, 100, 100,
            12, 15, 12, 7, 7, 7,

            getBasicAttack(),
            getPOWBoostedAttack()
    };
}


Player getRogueTemplate() {
    return {
            "", "Rogue", NCOLOR_LGREEN,
            standard_player_sprite, 1, 0,
            100, 100, 100,
            100, 100, 100,
            10, 13, 13, 10, 7, 7,

            getBasicAttack(),
            getAGIPOWBoostedAttack()
    };
}

Player getRangerTemplate() {
    return {
            "", "Ranger", NCOLOR_GREEN,
            standard_player_sprite, 1, 0,
            100, 100, 100,
            100, 100, 100,
            7, 12, 15, 12, 7, 7,

            getBasicAttack(),
            getAGIBoostedAttack()
    };
}


Player getDruidTemplate() {
    return {
            "", "Druid", NCOLOR_CYAN,
            standard_player_sprite, 1, 0,
            100, 100, 100,
            100, 100, 100,
            7, 10, 13, 13, 10, 7,

            getBasicAttack(),
            getFOCAGIBoostedAttack()
    };
}


Player getSorcererTemplate() {
    return {
            "", "Sorcerer", NCOLOR_AMARINE,
            standard_player_sprite, 1, 0,
            100, 100, 100,
            100, 100, 100,
            7, 7, 12, 15, 12, 7,

            getBasicAttack(),
            getFOCBoostedAttack()
    };
}


Player getBardTemplate() {
    return {
            "", "Bard", NCOLOR_LBLUE,
            standard_player_sprite, 1, 0,
            100, 100, 100,
            100, 100, 100,
            7, 7, 10, 13, 13, 10,

            getBasicAttack(),
            getINSFOCBoostedAttack()
    };
}


Player getWizardTemplate() {
    return {
            "", "Wizard", NCOLOR_BLUE,
            standard_player_sprite, 1, 0,
            100, 100, 100,
            100, 100, 100,
            7, 7, 7, 12, 15, 12,

            getBasicAttack(),
            getINSBoostedAttack()
    };
}

Player getMonkTemplate() {
    return {
            "", "Monk", NCOLOR_PURPLE,
            standard_player_sprite, 1, 0,
            100, 100, 100,
            100, 100, 100,
            10, 7, 7, 10, 13, 13,

            getBasicAttack(),
            getBELINSBoostedAttack()
    };
}


Player getClericTemplate() {
    return {
            "", "Cleric", NCOLOR_PINK,
            standard_player_sprite, 1, 0,
            100, 100, 100,
            100, 100, 100,
            12, 7, 7, 7, 12, 15,

            getBasicAttack(),
            getBELBoostedAttack()
    };
}


Player getPaladinTemplate() {
    return {
            "", "Paladin", NCOLOR_MAGENTA,
            standard_player_sprite, 1, 0,
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

