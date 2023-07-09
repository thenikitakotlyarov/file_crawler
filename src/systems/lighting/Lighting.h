#pragma once


#include <cmath>

#include "helpers.h"
#include "constants.h"


using namespace std;




class LightSystem {
public:
    LightSystem();

    ~LightSystem();

    int running;

    void CleanUp();

    void Update();


    set<pair<int,int>> castLight(
            const Frame &frame,
            Position pos,
            int radius,
            double falloff);

    Frame addPointLight(Frame frame, GameMap &game_map, Position light_pos,
                               int radius, char temperature,//temperature can be 'w' for warm, 'c' for cold, or defaults to neutral
                               int start_y, int start_x);


    Frame renderLighting(Frame frame,
                         GameMap &game_map, Position player_pos, int player_light_radius,
                         int start_y, int start_x, int end_y, int end_x);

private:
    bool Initialize();
    double update_phase = 1;

    double flickerSpeed = 0.1; // Adjust this value to control the flicker speed
    double flickerDamping = 0.5; // Adjust this value to control the flicker damping

    double previous_flicker_intensity = 1; // Holds the flicker intensity from the last frame

    map<pair<int, int>, double> pixel_intensities;
    double light_source_intensity;


    pair<Color, Color> degradeGreyscale(Color &this_color, Color &bg_color) const;
    pair<Color, Color> degradeWarm(Color &this_color, Color &bg_color) const;
};
