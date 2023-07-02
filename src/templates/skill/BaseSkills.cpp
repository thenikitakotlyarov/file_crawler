#include "BaseSkills.h"

Skill *getBasicAttack() {
    unsigned long id = 0;

    string name = "Basic Attack";

    vector<vector<wstring>> icon = {
            {
                    L" ", L" ", L"▞", L" ", L" ", L" ", L" ", L" "
            },
            {
                    L"0", L"═", L"A", L"t", L"t", L"a", L"c", L"k"
            },
            {
                    L" ", L" ", L"▚", L" ", L" ", L" ", L" ", L" "
            },
    };


    Skill skill = {
            id, name, icon,
            [](Player &self, Position &player_pos, const set<pair<int, int>> player_fov,
               map<Entity, Monster> monsters, map<Entity, Position> positions)
                    -> pair<int, vector<Intent>> {
                int cost = 2;
                vector<Intent> attack_targets;

                // Player primary attack
                if (self.current_energy > cost) {
                    for (const auto &entry: monsters) {
                        const int player_max_arc = self.dexterity / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.dexterity
                            && abs(player_pos.y - monsterPosition.y) <= self.dexterity
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 3;
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


                return make_pair(cost, attack_targets);
            }


    };

    return new Skill(skill);
}


Skill *getAverageBoostedAttack() {
    unsigned long id = 0;

    string name = "Special Attack";

    vector<vector<wstring>> icon = {
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
            {
                    L"S", L"p", L"e", L"c", L"i", L"a", L"l", L"!" //Special!
            },
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
    };


    Skill skill = {
            id, name, icon,
            [](Player &self, Position &player_pos, const set<pair<int, int>> player_fov,
               map<Entity, Monster> monsters, map<Entity, Position> positions)
                    -> pair<int, vector<Intent>> {
                int cost = 2;
                vector<Intent> attack_targets;

                // Player primary attack
                if (self.current_energy > cost) {
                    for (const auto &entry: monsters) {
                        const int player_max_arc = self.dexterity / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.dexterity
                            && abs(player_pos.y - monsterPosition.y) <= self.dexterity
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.strength);
                            int bonus =
                                    (self.strength
                                     + self.constitution
                                     + self.faith
                                     + self.intelligence
                                     + self.willpower
                                     + self.dexterity)
                                    / 6;
                            attack_targets.push_back({
                                                             monsterEntity,
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage+bonus
                                                     });
                            hit_monsters++;

                            if (hit_monsters > player_max_arc) break;
                        }
                    }
                }


                return make_pair(cost, attack_targets);
            }


    };

    return new Skill(skill);
}


Skill *getDexStrBoostedAttack() {
    unsigned long id = 0;

    string name = "Special Attack";

    vector<vector<wstring>> icon = {
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
            {
                    L"S", L"p", L"e", L"c", L"i", L"a", L"l", L"!" //Special!
            },
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
    };


    Skill skill = {
            id, name, icon,
            [](Player &self, Position &player_pos, const set<pair<int, int>> player_fov,
               map<Entity, Monster> monsters, map<Entity, Position> positions)
                    -> pair<int, vector<Intent>> {
                int cost = 2;
                vector<Intent> attack_targets;

                // Player primary attack
                if (self.current_energy > cost) {
                    for (const auto &entry: monsters) {
                        const int player_max_arc = self.dexterity / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.dexterity
                            && abs(player_pos.y - monsterPosition.y) <= self.dexterity
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.strength);
                            int bonus =
                                    (self.strength
                                     + self.dexterity)
                                    / 2;
                            attack_targets.push_back({
                                                             monsterEntity,
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage+bonus
                                                     });
                            hit_monsters++;

                            if (hit_monsters > player_max_arc) break;
                        }
                    }
                }


                return make_pair(cost, attack_targets);
            }


    };

    return new Skill(skill);
}


Skill *getStrBoostedAttack() {
    unsigned long id = 0;

    string name = "Special Attack";

    vector<vector<wstring>> icon = {
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
            {
                    L"S", L"p", L"e", L"c", L"i", L"a", L"l", L"!" //Special!
            },
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
    };


    Skill skill = {
            id, name, icon,
            [](Player &self, Position &player_pos, const set<pair<int, int>> player_fov,
               map<Entity, Monster> monsters, map<Entity, Position> positions)
                    -> pair<int, vector<Intent>> {
                int cost = 2;
                vector<Intent> attack_targets;

                // Player primary attack
                if (self.current_energy > cost) {
                    for (const auto &entry: monsters) {
                        const int player_max_arc = self.dexterity / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.dexterity
                            && abs(player_pos.y - monsterPosition.y) <= self.dexterity
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.strength);
                            int bonus = self.strength;
                            attack_targets.push_back({
                                                             monsterEntity,
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage+bonus
                                                     });
                            hit_monsters++;

                            if (hit_monsters > player_max_arc) break;
                        }
                    }
                }


                return make_pair(cost, attack_targets);
            }


    };

    return new Skill(skill);
}


Skill *getStrConBoostedAttack() {
    unsigned long id = 0;

    string name = "Special Attack";

    vector<vector<wstring>> icon = {
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
            {
                    L"S", L"p", L"e", L"c", L"i", L"a", L"l", L"!" //Special!
            },
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
    };


    Skill skill = {
            id, name, icon,
            [](Player &self, Position &player_pos, const set<pair<int, int>> player_fov,
               map<Entity, Monster> monsters, map<Entity, Position> positions)
                    -> pair<int, vector<Intent>> {
                int cost = 2;
                vector<Intent> attack_targets;

                // Player primary attack
                if (self.current_energy > cost) {
                    for (const auto &entry: monsters) {
                        const int player_max_arc = self.dexterity / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.dexterity
                            && abs(player_pos.y - monsterPosition.y) <= self.dexterity
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.strength);
                            int bonus =
                                    (self.strength
                                     + self.constitution)
                                    / 2;
                            attack_targets.push_back({
                                                             monsterEntity,
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage+bonus
                                                     });
                            hit_monsters++;

                            if (hit_monsters > player_max_arc) break;
                        }
                    }
                }


                return make_pair(cost, attack_targets);
            }


    };

    return new Skill(skill);
}


Skill *getConBoostedAttack() {
    unsigned long id = 0;

    string name = "Special Attack";

    vector<vector<wstring>> icon = {
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
            {
                    L"S", L"p", L"e", L"c", L"i", L"a", L"l", L"!" //Special!
            },
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
    };


    Skill skill = {
            id, name, icon,
            [](Player &self, Position &player_pos, const set<pair<int, int>> player_fov,
               map<Entity, Monster> monsters, map<Entity, Position> positions)
                    -> pair<int, vector<Intent>> {
                int cost = 2;
                vector<Intent> attack_targets;

                // Player primary attack
                if (self.current_energy > cost) {
                    for (const auto &entry: monsters) {
                        const int player_max_arc = self.dexterity / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.dexterity
                            && abs(player_pos.y - monsterPosition.y) <= self.dexterity
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.strength);
                            int bonus = self.constitution;
                            attack_targets.push_back({
                                                             monsterEntity,
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage+bonus
                                                     });
                            hit_monsters++;

                            if (hit_monsters > player_max_arc) break;
                        }
                    }
                }


                return make_pair(cost, attack_targets);
            }


    };

    return new Skill(skill);
}


Skill *getConFaiBoostedAttack() {
    unsigned long id = 0;

    string name = "Special Attack";

    vector<vector<wstring>> icon = {
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
            {
                    L"S", L"p", L"e", L"c", L"i", L"a", L"l", L"!" //Special!
            },
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
    };


    Skill skill = {
            id, name, icon,
            [](Player &self, Position &player_pos, const set<pair<int, int>> player_fov,
               map<Entity, Monster> monsters, map<Entity, Position> positions)
                    -> pair<int, vector<Intent>> {
                int cost = 2;
                vector<Intent> attack_targets;

                // Player primary attack
                if (self.current_energy > cost) {
                    for (const auto &entry: monsters) {
                        const int player_max_arc = self.dexterity / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.dexterity
                            && abs(player_pos.y - monsterPosition.y) <= self.dexterity
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.strength);
                            int bonus =
                                    (self.constitution
                                     + self.faith)
                                    / 2;
                            attack_targets.push_back({
                                                             monsterEntity,
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage+bonus
                                                     });
                            hit_monsters++;

                            if (hit_monsters > player_max_arc) break;
                        }
                    }
                }


                return make_pair(cost, attack_targets);
            }


    };

    return new Skill(skill);
}


Skill *getFaiBoostedAttack() {
    unsigned long id = 0;

    string name = "Special Attack";

    vector<vector<wstring>> icon = {
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
            {
                    L"S", L"p", L"e", L"c", L"i", L"a", L"l", L"!" //Special!
            },
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
    };


    Skill skill = {
            id, name, icon,
            [](Player &self, Position &player_pos, const set<pair<int, int>> player_fov,
               map<Entity, Monster> monsters, map<Entity, Position> positions)
                    -> pair<int, vector<Intent>> {
                int cost = 2;
                vector<Intent> attack_targets;

                // Player primary attack
                if (self.current_energy > cost) {
                    for (const auto &entry: monsters) {
                        const int player_max_arc = self.dexterity / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.dexterity
                            && abs(player_pos.y - monsterPosition.y) <= self.dexterity
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.strength);
                            int bonus = self.faith;
                            attack_targets.push_back({
                                                             monsterEntity,
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage+bonus
                                                     });
                            hit_monsters++;

                            if (hit_monsters > player_max_arc) break;
                        }
                    }
                }


                return make_pair(cost, attack_targets);
            }


    };

    return new Skill(skill);
}


Skill *getFaiIntBoostedAttack() {
    unsigned long id = 0;

    string name = "Special Attack";

    vector<vector<wstring>> icon = {
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
            {
                    L"S", L"p", L"e", L"c", L"i", L"a", L"l", L"!" //Special!
            },
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
    };


    Skill skill = {
            id, name, icon,
            [](Player &self, Position &player_pos, const set<pair<int, int>> player_fov,
               map<Entity, Monster> monsters, map<Entity, Position> positions)
                    -> pair<int, vector<Intent>> {
                int cost = 2;
                vector<Intent> attack_targets;

                // Player primary attack
                if (self.current_energy > cost) {
                    for (const auto &entry: monsters) {
                        const int player_max_arc = self.dexterity / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.dexterity
                            && abs(player_pos.y - monsterPosition.y) <= self.dexterity
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.strength);
                            int bonus =
                                    (self.faith
                                     + self.intelligence)
                                    / 2;
                            attack_targets.push_back({
                                                             monsterEntity,
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage+bonus
                                                     });
                            hit_monsters++;

                            if (hit_monsters > player_max_arc) break;
                        }
                    }
                }


                return make_pair(cost, attack_targets);
            }


    };

    return new Skill(skill);
}


Skill *getIntBoostedAttack() {
    unsigned long id = 0;

    string name = "Special Attack";

    vector<vector<wstring>> icon = {
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
            {
                    L"S", L"p", L"e", L"c", L"i", L"a", L"l", L"!" //Special!
            },
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
    };


    Skill skill = {
            id, name, icon,
            [](Player &self, Position &player_pos, const set<pair<int, int>> player_fov,
               map<Entity, Monster> monsters, map<Entity, Position> positions)
                    -> pair<int, vector<Intent>> {
                int cost = 2;
                vector<Intent> attack_targets;

                // Player primary attack
                if (self.current_energy > cost) {
                    for (const auto &entry: monsters) {
                        const int player_max_arc = self.dexterity / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.dexterity
                            && abs(player_pos.y - monsterPosition.y) <= self.dexterity
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.strength);
                            int bonus = self.intelligence;
                            attack_targets.push_back({
                                                             monsterEntity,
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage+bonus
                                                     });
                            hit_monsters++;

                            if (hit_monsters > player_max_arc) break;
                        }
                    }
                }


                return make_pair(cost, attack_targets);
            }


    };

    return new Skill(skill);
}


Skill *getIntWilBoostedAttack() {
    unsigned long id = 0;

    string name = "Special Attack";

    vector<vector<wstring>> icon = {
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
            {
                    L"S", L"p", L"e", L"c", L"i", L"a", L"l", L"!" //Special!
            },
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
    };


    Skill skill = {
            id, name, icon,
            [](Player &self, Position &player_pos, const set<pair<int, int>> player_fov,
               map<Entity, Monster> monsters, map<Entity, Position> positions)
                    -> pair<int, vector<Intent>> {
                int cost = 2;
                vector<Intent> attack_targets;

                // Player primary attack
                if (self.current_energy > cost) {
                    for (const auto &entry: monsters) {
                        const int player_max_arc = self.dexterity / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.dexterity
                            && abs(player_pos.y - monsterPosition.y) <= self.dexterity
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.strength);
                            int bonus =
                                    (self.intelligence
                                     + self.willpower)
                                    /2;
                            attack_targets.push_back({
                                                             monsterEntity,
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage+bonus
                                                     });
                            hit_monsters++;

                            if (hit_monsters > player_max_arc) break;
                        }
                    }
                }


                return make_pair(cost, attack_targets);
            }


    };

    return new Skill(skill);
}


Skill *getWilBoostedAttack() {
    unsigned long id = 0;

    string name = "Special Attack";

    vector<vector<wstring>> icon = {
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
            {
                    L"S", L"p", L"e", L"c", L"i", L"a", L"l", L"!" //Special!
            },
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
    };


    Skill skill = {
            id, name, icon,
            [](Player &self, Position &player_pos, const set<pair<int, int>> player_fov,
               map<Entity, Monster> monsters, map<Entity, Position> positions)
                    -> pair<int, vector<Intent>> {
                int cost = 2;
                vector<Intent> attack_targets;

                // Player primary attack
                if (self.current_energy > cost) {
                    for (const auto &entry: monsters) {
                        const int player_max_arc = self.dexterity / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.dexterity
                            && abs(player_pos.y - monsterPosition.y) <= self.dexterity
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.strength);
                            int bonus = self.willpower;
                            attack_targets.push_back({
                                                             monsterEntity,
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage+bonus
                                                     });
                            hit_monsters++;

                            if (hit_monsters > player_max_arc) break;
                        }
                    }
                }


                return make_pair(cost, attack_targets);
            }


    };

    return new Skill(skill);
}


Skill *getWilDexBoostedAttack() {
    unsigned long id = 0;

    string name = "Special Attack";

    vector<vector<wstring>> icon = {
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
            {
                    L"S", L"p", L"e", L"c", L"i", L"a", L"l", L"!" //Special!
            },
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
    };


    Skill skill = {
            id, name, icon,
            [](Player &self, Position &player_pos, const set<pair<int, int>> player_fov,
               map<Entity, Monster> monsters, map<Entity, Position> positions)
                    -> pair<int, vector<Intent>> {
                int cost = 2;
                vector<Intent> attack_targets;

                // Player primary attack
                if (self.current_energy > cost) {
                    for (const auto &entry: monsters) {
                        const int player_max_arc = self.dexterity / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.dexterity
                            && abs(player_pos.y - monsterPosition.y) <= self.dexterity
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.strength);
                            int bonus =
                                    (self.willpower
                                     + self.dexterity)
                                    / 2;
                            attack_targets.push_back({
                                                             monsterEntity,
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage+bonus
                                                     });
                            hit_monsters++;

                            if (hit_monsters > player_max_arc) break;
                        }
                    }
                }


                return make_pair(cost, attack_targets);
            }


    };

    return new Skill(skill);
}


Skill *getDexBoostedAttack() {
    unsigned long id = 0;

    string name = "Special Attack";

    vector<vector<wstring>> icon = {
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
            {
                    L"S", L"p", L"e", L"c", L"i", L"a", L"l", L"!" //Special!
            },
            {
                    L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"
            },
    };


    Skill skill = {
            id, name, icon,
            [](Player &self, Position &player_pos, const set<pair<int, int>> player_fov,
               map<Entity, Monster> monsters, map<Entity, Position> positions)
                    -> pair<int, vector<Intent>> {
                int cost = 2;
                vector<Intent> attack_targets;

                // Player primary attack
                if (self.current_energy > cost) {
                    for (const auto &entry: monsters) {
                        const int player_max_arc = self.dexterity / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.dexterity
                            && abs(player_pos.y - monsterPosition.y) <= self.dexterity
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.strength);
                            int bonus = self.dexterity;
                            attack_targets.push_back({
                                                             monsterEntity,
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage+bonus
                                                     });
                            hit_monsters++;

                            if (hit_monsters > player_max_arc) break;
                        }
                    }
                }


                return make_pair(cost, attack_targets);
            }


    };

    return new Skill(skill);
}