#pragma once

#include <cmath>
#include <unordered_set>
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

    void Update(EntityMap *entity_map);

    void setMaps(GameMap *game_map, EntityMap *entity_map, unsigned long x, unsigned long y);

    void populateMaps();

    unordered_set<Position, PositionHash>
    castLight(Position pos, int radius, int start_x, int start_y, int end_x, int end_y);

    pair<Color, Color> degradeGrey(pair<Color, Color> &this_color) const;

    pair<Color, Color> degradeWarm(pair<Color, Color> &this_color) const;

    pair<Color, Color> degradeCold(pair<Color, Color> &this_color) const;

    pair<Color, Color> degradeColorTemperature(pair<Color, Color> &this_color, char color_temperature) const;

    uint8_t getFlickerVal(uint8_t val, double flicker) const;

    pair<Color, Color> flickerLight(pair<Color, Color> &this_color, const double flicker) const;

    Frame addPointLight(Frame frame, Position light_pos, Light light, int start_y, int start_x);

    Frame addAmbientLight(Frame frame, Position light_pos, Color color, int start_y, int start_x);

    Frame renderLighting2D(Frame frame, Position player_pos, int player_light_radius, int start_y, int start_x, int end_y,
                           int end_x);
//    void castShadows(Frame& frame, Position light_pos, int radius, int start_x, int start_y, int end_x, int end_y);
//    void updateLightingForTimeOfDay(int hour);

private:
    bool Initialize();

    double update_phase = 1;
    GameMap *currentGameMap;
    EntityMap *currentEntityMap;
    LightMap staticMap;
    LightMap dynamicMap;
    Light noLight = {{0, 0, 0}, 'n', 0, 0.0, 1.0, 1.0};
    Light overworldLight = {{248, 248, 248}, 'n', 1, 1.0, 1.0, 0.7};
    Light underworldLight = {{32, 0, 0}, 'n', 0, 0.3, 0.0, 0.3};
    Light emissiveLight = {{32, 64, 128}, 'n', 3, 0.3, 0.0, 0.1};
};

