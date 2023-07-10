#pragma once

#include "components/map/GameMap.h"
#include "templates/item/BaseItems.h"
#include "templates/enemy/BaseEnemies.h"

using namespace std;

#include "helpers.h"
#include "constants.h"

class MapSystem {
public:
    MapSystem();

    ~MapSystem();

    int running;

    void CleanUp();

    void Update();

    GameMap *genCave(int height, int width);

    void unveilMap(GameMap *game_map, const set<pair<int, int>> &current_fov);

    void veilMap(GameMap *game_map, const set<pair<int, int>> &current_fov);

    void forgetMap(GameMap *game_map, const pair<int, int> player_pos, int mem_mult);

    static Frame renderMap2D(Frame frame, GameMap *current_map,
                             int start_y, int start_x,
                             int end_y, int end_x);

    static Frame renderMap3D(Frame frame,GameMap *current_map,
                             int start_y, int start_x,
                             int end_y, int end_x);

private:
    bool Initialize();


    static Color get_tile_color(const vector<Color> &swatch);
};
