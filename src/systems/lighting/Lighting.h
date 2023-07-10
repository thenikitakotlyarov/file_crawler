#pragma once


#include <cmath>

#include "helpers.h"
#include "constants.h"
#include "components/map/GameMap.h"
#include "components/map/EntityMap.h"
#include "components/map/LightMap.h"


using namespace std;


class LightSystem {
public:
    LightSystem();

    ~LightSystem();

    int running;

    void CleanUp();

    void Update(const EntityMap& entity_map);


    void setMaps(const GameMap &game_map, const EntityMap &entity_map, unsigned long x, unsigned long y);

    void populateMaps();

    set<pair<int, int>> castLight(
            Position pos,
            int radius);


    pair<Color, Color> degradeGreyscale(Color &this_color, Color &bg_color) const;

    pair<Color, Color> degradeWarm(Color &this_color, Color &bg_color) const;

    Frame addPointLight(Frame frame, Position light_pos,
                        int radius,
                        char temperature,//temperature can be 'w' for warm, 'c' for cold, or defaults to neutral
                        int start_y, int start_x);

    Frame addWashLight(Frame frame, Position light_pos,
                       Color color,
                       int start_y, int start_x);


    Frame renderLighting(Frame frame,
                         Position player_pos, int player_light_radius,
                         int start_y, int start_x, int end_y, int end_x);

private:
    bool Initialize();


    double update_phase = 1;


    GameMap currentGameMap;
    EntityMap currentEntityMap;


    LightMap staticMap;
    LightMap dynamicMap;


    Light noLight = {
            {0, 0, 0},
            0.0, 0.0, 0.0
    };

    Light overworldLight = {
            {248, 248, 248},
            1.0, 0.0, 0.7
    };

    Light underworldLight = {
            {32, 0, 0},
            0.3, 0.0, 0.3
    };

    Light emissiveLight = {
            {32, 64, 128},
            0.5, 0.3, 0.2
    };


};
