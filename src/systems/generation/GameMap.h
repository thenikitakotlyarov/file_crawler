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

    GameMap genCave(int height, int width);

    static GameMap& unveilMap(GameMap &game_map, const set<pair<int, int>> &current_fov);

    static GameMap& veilMap(GameMap &game_map, const set<pair<int, int>> &current_fov);

    static GameMap& forgetMap(GameMap &game_map, const pair<int,int> player_pos, int mem_mult);

    static Frame renderMap2D(Frame frame, const GameMap &current_map,
                      int start_y, int start_x,
                      int end_y, int end_x);

    static Frame renderMap3D(Frame frame, const GameMap &current_map,
                             int start_y, int start_x,
                             int end_y, int end_x);

private:
    bool Initialize();


    static int get_tile_color(const vector<int>& swatch);
};
