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
                {NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_LGREY, NCOLOR_BLACK, NCOLOR_BLACK},
                {NCOLOR_BLACK, NCOLOR_MGREY, NCOLOR_MGREY, NCOLOR_MGREY, NCOLOR_BLACK},
                {NCOLOR_BLACK, NCOLOR_DGREY, NCOLOR_MGREY, NCOLOR_DGREY, NCOLOR_BLACK},
                {NCOLOR_BLACK, NCOLOR_BLACK, NCOLOR_MGREY, NCOLOR_BLACK, NCOLOR_BLACK},
                {NCOLOR_BLACK, NCOLOR_DGREY, NCOLOR_BLACK, NCOLOR_DGREY, NCOLOR_BLACK}

        }

};
Player getAdventurerTemplate() {
    return {
            "Link", "Adventurer", NCOLOR_MGREY,
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
            "Conan", "Barbarian", NCOLOR_RED,
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
            "Geralt", "Duelist", NCOLOR_ORANGE,
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
            "Ryu", "Fighter", NCOLOR_YELLOW,
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
            "Garrett", "Rogue", NCOLOR_LGREEN,
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
            "Artemis", "Ranger", NCOLOR_GREEN,
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
            "Malfurion", "Druid", NCOLOR_CYAN,
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
            "Gandalf", "Sorcerer", NCOLOR_AMARINE,
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
            "Garrick", "Bard", NCOLOR_LBLUE,
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
            "Merlin", "Wizard", NCOLOR_BLUE,
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
            "Sabin", "Monk", NCOLOR_PURPLE,
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
            "Alistair", "Cleric", NCOLOR_PINK,
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
            "Tyrael", "Paladin", NCOLOR_MAGENTA,
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

