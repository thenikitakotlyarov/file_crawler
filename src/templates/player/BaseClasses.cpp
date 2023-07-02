#include "BaseClasses.h"

Player getAdventurerTemplate() {
    Player Adventurer = {
            "", "Adventurer", COLOR_WHITE,
            1, 0,
            100, 100, 100,
            100, 100, 100,
            4, 4, 4, 4, 4, 4,
            [](Player &self, Position &player_pos, const set<pair<int,int>> player_fov,
                    map<Entity, Monster> monsters, map<Entity,Position> positions)
                    -> vector<Intent>  {
                vector<Intent> attack_targets;

                // Player primary attack
                if (self.current_energy >= 2) {
                    self.current_energy -= 2;
                    for (const auto &entry: monsters) {
                        const int player_max_arc = self.dexterity / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.dexterity
                            && abs(player_pos.y - monsterPosition.y) <= self.dexterity
                            && is_position_in_fov(monsterPosition,player_fov)
                            && self.current_energy >= 5) {
                            self.current_energy -= 5;
                            int damage = get_random_int(0, self.strength);
                            attack_targets.push_back({
                                monsterEntity,
                                IntentType::Attack,
                                {},
                                monsterEntity,
                                damage
                            });
                            hit_monsters++;

                            if (hit_monsters > player_max_arc) break;
                        }
                    }
                }


                return attack_targets;
            }


    };


    return Adventurer;
}


Player getBarbarianTemplate() {
    Player Barbarian = {
            "", "Barbarian", COLOR_RED,
            1, 0,
            100, 100, 100,
            100, 100, 100,
            4, 4, 4, 4, 4, 4
    };

    return Barbarian;
}


Player getBladeTemplate() {
    Player Blade = {
            "", "Blade", COLOR_ORANGE,
            1, 0,
            100, 100, 100,
            100, 100, 100,
            4, 4, 4, 4, 4, 4
    };

    return Blade;
}


Player getRogueTemplate() {
    Player Rogue = {
            "", "Rogue", COLOR_YELLOW,
            1, 0,
            100, 100, 100,
            100, 100, 100,
            4, 4, 4, 4, 4, 4
    };

    return Rogue;
}


Player getScoutTemplate() {
    Player Scout = {
            "", "Scout", COLOR_LGREEN,
            1, 0,
            100, 100, 100,
            100, 100, 100,
            4, 4, 4, 4, 4, 4
    };

    return Scout;
}


Player getRangerTemplate() {
    Player Ranger = {
            "", "Ranger", COLOR_GREEN,
            1, 0,
            100, 100, 100,
            100, 100, 100,
            4, 4, 4, 4, 4, 4
    };

    return Ranger;
}


Player getNecromancerTemplate() {
    Player Necromancer = {
            "", "Necromancer", COLOR_CYAN,
            1, 0,
            100, 100, 100,
            100, 100, 100,
            4, 4, 4, 4, 4, 4
    };

    return Necromancer;
}


Player getSorcererTemplate() {
    Player Sorcerer = {
            "", "Sorcerer", COLOR_AMARINE,
            1, 0,
            100, 100, 100,
            100, 100, 100,
            4, 4, 4, 4, 4, 4
    };

    return Sorcerer;
}


Player getMonkTemplate() {
    Player Monk = {
            "", "Monk", COLOR_LBLUE,
            1, 0,
            100, 100, 100,
            100, 100, 100,
            4, 4, 4, 4, 4, 4
    };

    return Monk;
}


Player getWizardTemplate() {
    Player Wizard = {
            "", "Wizard", COLOR_BLUE,
            1, 0,
            100, 100, 100,
            100, 100, 100,
            4, 4, 4, 4, 4, 4
    };

    return Wizard;
}


Player getClericTemplate() {
    Player Cleric = {
            "", "Cleric", COLOR_PURPLE,
            1, 0,
            100, 100, 100,
            100, 100, 100,
            4, 4, 4, 4, 4, 4
    };

    return Cleric;
}


Player getSpellswordTemplate() {
    Player Spellsword = {
            "", "Spellsword", COLOR_PINK,
            1, 0,
            100, 100, 100,
            100, 100, 100,
            4, 4, 4, 4, 4, 4
    };

    return Spellsword;
}


Player getPaladinTemplate() {
    Player Paladin = {
            "", "Paladin", COLOR_MAGENTA,
            1, 0,
            100, 100, 100,
            100, 100, 100,
            4, 4, 4, 4, 4, 4
    };

    return Paladin;
}


Player getRandomTemplate() {
    int choice = get_random_int(0, 11);
    Player Random;
    switch (choice) {
        case (0):
            Random = getBarbarianTemplate();
            break;
        case (1):
            Random = getBladeTemplate();
            break;
        case (2):
            Random = getRogueTemplate();
            break;
        case (3):
            Random = getScoutTemplate();
            break;
        case (4):
            Random = getRangerTemplate();
            break;
        case (5):
            Random = getNecromancerTemplate();
            break;
        case (6):
            Random = getSorcererTemplate();
            break;
        case (7):
            Random = getMonkTemplate();
            break;
        case (8):
            Random = getWizardTemplate();
            break;
        case (9):
            Random = getClericTemplate();
            break;
        case (10):
            Random = getSpellswordTemplate();
            break;
        case (11):
            Random = getPaladinTemplate();
            break;
    }

    return Random;
}

