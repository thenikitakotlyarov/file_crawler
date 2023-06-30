# include "BaseEnemies.h"

Monster getBasicMonster() {
    float rating = 1.0f;
    pair<int, int> attack_power_dis = {rating*1, rating*20};
    pair<int, int> attack_range_dis = {rating*1, rating*7};
    pair<int, int> chase_dis = {rating*7, rating*25};
    pair<int, int> attack_cooldown_dis = {rating*1, rating*7};
    pair<int, int> health_dis = {rating*5, rating*25};

    const int monster_attack_power = get_random_int(attack_power_dis.first,attack_power_dis.second);
    const int monster_attack_range = get_random_int(attack_range_dis.first,attack_range_dis.second);
    const int monster_chase_range = get_random_int(chase_dis.first,chase_dis.second);
    const int monster_attack_cooldown = FPS*get_random_int(attack_cooldown_dis.first,attack_cooldown_dis.second);
    const int monster_health = get_random_int(health_dis.first,health_dis.second);


    return {
            "Monster",
            get_random_character(MONSTER_HIGH_HP_TILES),
            get_random_character(MONSTER_MID_HP_TILES),
            get_random_character(MONSTER_LOW_HP_TILES),
            COLOR_RED, 1,
            monster_attack_power, monster_attack_range, monster_chase_range, monster_attack_cooldown,
            monster_health, monster_health,
            [](Monster &self, Position &monster_pos, Player &player_stats, Position &player_pos, GameMap &game_map)->Position  {
                Position new_pos;
                self.cooldown = max(0, self.cooldown - 1);
                int dx = monster_pos.x - player_pos.x;
                int dy = monster_pos.y - player_pos.y;
                double distance = sqrt(dx * dx + dy * dy);

                if (distance <= self.chaseRadius) {

                    vector<Node> path = aStar(monster_pos, player_pos, game_map);
                    new_pos.x = monster_pos.x;
                    new_pos.y = monster_pos.y;
                    // Move the monster to the next node in the path if it exists
                    if (!path.empty()) {
                        // introduce a little noise to monster path
                        new_pos.x = max(1, min(WIDTH - 1, path[1].x));
                        new_pos.y = max(1, min(HEIGHT - 1, path[1].y));
                    } else {
                        int wander_x = 0, wander_y = 0;
                        // introduce a little noise to monster path
                        if (get_random_int(1, FPS) == FPS) {
                            wander_x = get_random_int(-1, 1);
                        }
                        if (get_random_int(1, FPS) == FPS) {
                            wander_y = get_random_int(-1, 1);
                        }
                        new_pos.x = max(1, min(WIDTH - 1, monster_pos.x + wander_x));
                        new_pos.y = max(1, min(HEIGHT - 1, monster_pos.y + wander_y));

                    }
                }


                return new_pos;
            }
    };
}