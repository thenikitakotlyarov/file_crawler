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
                            cost += 5;
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