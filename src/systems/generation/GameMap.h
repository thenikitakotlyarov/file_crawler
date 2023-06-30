#pragma once

#include "components/GameMap.h"

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

    GameMap unveilMap(GameMap game_map,  set<pair<int,int>> current_fov);
    GameMap reveilMap(GameMap game_map,  set<pair<int,int>> current_fov);

    Frame renderMap2D(Frame frame, GameMap current_map,
                      int start_y, int start_x,
                      int end_y, int end_x);


private:
    bool Initialize();


    int get_tile_color(const vector<int> swatch);
};
