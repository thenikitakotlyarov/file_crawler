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
                        const int player_max_arc = self.agility / 10;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.agility
                            && abs(player_pos.y - monsterPosition.y) <= self.agility
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 3;
                            int damage = get_random_int(0, self.power);
                            attack_targets.push_back({
                                                             {1},
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage
                                                     });

                            break;
                        }
                    }
                }


                return make_pair(cost, attack_targets);
            }


    };

    return new Skill(skill);
}


Skill *getAVGBoostedAttack() {
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
                        const int player_max_arc = self.agility / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.agility
                            && abs(player_pos.y - monsterPosition.y) <= self.agility
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.power);
                            int bonus =
                                    (self.power
                                     + self.vitality
                                     + self.belief
                                     + self.insight
                                     + self.focus
                                     + self.agility)
                                    / 6;
                            attack_targets.push_back({
                                                             {1},
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage + bonus
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


Skill *getAGIPOWBoostedAttack() {
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
                        const int player_max_arc = self.agility / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.agility
                            && abs(player_pos.y - monsterPosition.y) <= self.agility
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.power);
                            int bonus =
                                    (self.power
                                     + self.agility)
                                    / 2;
                            attack_targets.push_back({
                                                             {1},
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage + bonus
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


Skill *getPOWBoostedAttack() {
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
                        const int player_max_arc = self.agility / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.agility
                            && abs(player_pos.y - monsterPosition.y) <= self.agility
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.power);
                            int bonus = self.power;
                            attack_targets.push_back({
                                                             {1},
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage + bonus
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


Skill *getPOWVITBoostedAttack() {
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
                        const int player_max_arc = self.agility / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.agility
                            && abs(player_pos.y - monsterPosition.y) <= self.agility
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.power);
                            int bonus =
                                    (self.power
                                     + self.vitality)
                                    / 2;
                            attack_targets.push_back({
                                                             {1},
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage + bonus
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


Skill *getVITBoostedAttack() {
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
                        const int player_max_arc = self.agility / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.agility
                            && abs(player_pos.y - monsterPosition.y) <= self.agility
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.power);
                            int bonus = self.vitality;
                            attack_targets.push_back({
                                                             {1},
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage + bonus
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


Skill *getVITBELBoostedAttack() {
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
                        const int player_max_arc = self.agility / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.agility
                            && abs(player_pos.y - monsterPosition.y) <= self.agility
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.power);
                            int bonus =
                                    (self.vitality
                                     + self.belief)
                                    / 2;
                            attack_targets.push_back({
                                                             {1},
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage + bonus
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


Skill *getBELBoostedAttack() {
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
                        const int player_max_arc = self.agility / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.agility
                            && abs(player_pos.y - monsterPosition.y) <= self.agility
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.power);
                            int bonus = self.belief;
                            attack_targets.push_back({
                                                             {1},
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage + bonus
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


Skill *getBELINSBoostedAttack() {
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
                        const int player_max_arc = self.agility / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.agility
                            && abs(player_pos.y - monsterPosition.y) <= self.agility
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.power);
                            int bonus =
                                    (self.belief
                                     + self.insight)
                                    / 2;
                            attack_targets.push_back({
                                                             {1},
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage + bonus
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


Skill *getINSBoostedAttack() {
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
                        const int player_max_arc = self.agility / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.agility
                            && abs(player_pos.y - monsterPosition.y) <= self.agility
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.power);
                            int bonus = self.insight;
                            attack_targets.push_back({
                                                             {1},
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage + bonus
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


Skill *getINSFOCBoostedAttack() {
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
                        const int player_max_arc = self.agility / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.agility
                            && abs(player_pos.y - monsterPosition.y) <= self.agility
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.power);
                            int bonus =
                                    (self.insight
                                     + self.focus)
                                    / 2;
                            attack_targets.push_back({
                                                             {1},
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage + bonus
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


Skill *getFOCBoostedAttack() {
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
                        const int player_max_arc = self.agility / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.agility
                            && abs(player_pos.y - monsterPosition.y) <= self.agility
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.power);
                            int bonus = self.focus;
                            attack_targets.push_back({
                                                             {1},
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage + bonus
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


Skill *getFOCAGIBoostedAttack() {
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
                        const int player_max_arc = self.agility / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.agility
                            && abs(player_pos.y - monsterPosition.y) <= self.agility
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.power);
                            int bonus =
                                    (self.focus
                                     + self.agility)
                                    / 2;
                            attack_targets.push_back({
                                                             {1},
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage + bonus
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


Skill *getAGIBoostedAttack() {
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
                        const int player_max_arc = self.agility / 10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (abs(player_pos.x - monsterPosition.x) <= self.agility
                            && abs(player_pos.y - monsterPosition.y) <= self.agility
                            && is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;
                            int damage = get_random_int(0, self.power);
                            int bonus = self.agility;
                            attack_targets.push_back({
                                                             {1},
                                                             IntentType::Attack,
                                                             {},
                                                             monsterEntity,
                                                             damage + bonus
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

Skill *getFuriousCharge() {
    unsigned long id = 0;

    string name = "Furious Charge";

    vector<vector<wstring>> icon = {
            {L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"},
            {L"C", L"h", L"a", L"r", L"g", L"e", L"!", L" "},
            {L"⚞", L"═", L"═", L"═", L"═", L"═", L"═", L"⚟"},
    };


    Skill skill = {
            id, name, icon,
            [](Player &self, Position &player_pos, const set<pair<int, int>> player_fov,
               map<Entity, Monster> monsters, map<Entity, Position> positions)
                    -> pair<int, vector<Intent>> {
                int cost = 2;
                vector<Intent> intents;

                if (self.current_energy > cost) {
                    for (const auto &entry: monsters) {
                        Entity monsterEntity = entry.first;
                        Position monsterPosition = positions[monsterEntity];
                        Monster &monster = monsters[monsterEntity];

                        if (is_position_in_fov(monsterPosition, player_fov)
                            && self.current_energy >= cost + 5) {
                            cost += 10;

                            // Determine direction of the charge
                            pair<int, int> direction = {monsterPosition.x - player_pos.x,
                                                        monsterPosition.y - player_pos.y};

                            // Create move intent
                            Intent moveIntent = {{1}, IntentType::Move, direction};
                            intents.push_back(moveIntent);

                            // Calculate damage
                            int damage = self.power + (self.vitality / 2);

                            // Create attack intent
                            Intent attackIntent = {{1}, IntentType::Attack, {}, monsterEntity, damage};
                            intents.push_back(attackIntent);
                        }
                    }
                }

                return make_pair(cost, intents);
            }
    };

    return new Skill(skill);
}
