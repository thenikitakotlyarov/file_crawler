#include "Lighting.h"


unordered_map<Color, Color> GREYSCALE_LIGHT_FALLOFF_MAP{
        {NCOLOR_WHITE,   NCOLOR_LGREY},
        {NCOLOR_YELLOW,  NCOLOR_LGREY},
        {NCOLOR_AMARINE, NCOLOR_LGREY},
        {NCOLOR_PINK,    NCOLOR_LGREY},
        {NCOLOR_LGREY,   NCOLOR_GREY},
        {NCOLOR_ORANGE,  NCOLOR_GREY},
        {NCOLOR_LGREEN,  NCOLOR_GREY},
        {NCOLOR_CYAN,    NCOLOR_GREY},
        {NCOLOR_LBLUE,   NCOLOR_GREY},
        {NCOLOR_PURPLE,  NCOLOR_GREY},
        {NCOLOR_MAGENTA, NCOLOR_GREY},
        {NCOLOR_GREY,    NCOLOR_BLACK},
};


unordered_map<Color, Color> WARM_RED_LIGHT_FALLOFF_MAP{
        {NCOLOR_WHITE,  NCOLOR_YELLOW},
        {NCOLOR_YELLOW, NCOLOR_ORANGE},
        {NCOLOR_ORANGE, NCOLOR_RED},
        {NCOLOR_RED,    NCOLOR_GREY},
        {NCOLOR_GREY,   NCOLOR_BLACK},
};

unordered_map<Color, Color> COLD_RED_LIGHT_FALLOFF_MAP{
        {NCOLOR_WHITE,   NCOLOR_PINK},
        {NCOLOR_PINK,    NCOLOR_MAGENTA},
        {NCOLOR_MAGENTA, NCOLOR_RED},
        {NCOLOR_RED,     NCOLOR_GREY},
        {NCOLOR_GREY,    NCOLOR_BLACK},
};


unordered_map<Color, Color> WARM_GREEN_LIGHT_FALLOFF_MAP{
        {NCOLOR_WHITE,  NCOLOR_YELLOW},
        {NCOLOR_YELLOW, NCOLOR_LGREEN},
        {NCOLOR_LGREEN, NCOLOR_GREEN},
        {NCOLOR_GREEN,  NCOLOR_GREY},
        {NCOLOR_GREY,   NCOLOR_BLACK},
};

unordered_map<Color, Color> COLD_GREEN_LIGHT_FALLOFF_MAP{
        {NCOLOR_WHITE,   NCOLOR_AMARINE},
        {NCOLOR_AMARINE, NCOLOR_CYAN},
        {NCOLOR_CYAN,    NCOLOR_GREEN},
        {NCOLOR_GREEN,   NCOLOR_GREY},
        {NCOLOR_GREY,    NCOLOR_BLACK},
};

unordered_map<Color, Color> WARM_BLUE_LIGHT_FALLOFF_MAP{
        {NCOLOR_WHITE,  NCOLOR_PINK},
        {NCOLOR_PINK,   NCOLOR_PURPLE},
        {NCOLOR_PURPLE, NCOLOR_BLUE},
        {NCOLOR_BLUE,   NCOLOR_GREY},
        {NCOLOR_GREY,   NCOLOR_BLACK},
};

unordered_map<Color, Color> COLD_BLUE_LIGHT_FALLOFF_MAP{
        {NCOLOR_WHITE,   NCOLOR_AMARINE},
        {NCOLOR_AMARINE, NCOLOR_LBLUE},
        {NCOLOR_LBLUE,   NCOLOR_BLUE},
        {NCOLOR_BLUE,    NCOLOR_GREY},
        {NCOLOR_GREY,    NCOLOR_BLACK},
};


LightSystem::LightSystem() {
    bool init_out = Initialize();
    if (!init_out) {
        running = false;
    } else {
        running = true;
    }

}

LightSystem::~LightSystem() {
    CleanUp();
}

bool LightSystem::Initialize() {
    // Initialize systems
    return true;
}

void LightSystem::CleanUp() {
    // Clean up systems
}

void LightSystem::Update() {
    if (!get_random_int(0, 499)) { // 0.5% chance per frame
        light_source_intensity -= 0.1;  // decrease intensity by 10%
    } else {
        light_source_intensity += 0.02;     // slowly increase intensity
        if (light_source_intensity > 1) {
            light_source_intensity = 1; // cap at 1
        }
    }
}

set<pair<int,int>> LightSystem::castLight(
        const Frame &frame,
        Position pos,
        int radius,
        double falloff) {
    set<pair<int, int>> fov;
    for (int i = 0; i < 360; ++i) {
        double rad = i * (M_PI / 180.0);
        double dx = cos(rad), dy = sin(rad) / 2;
        double x = pos.x, y = pos.y;
        for (int j = 0; j <= radius; ++j) {
            int ix = round(x), iy = round(y);
            if (ix < 0 || iy < 0 || ix >= WIDTH || iy >= HEIGHT) break;
            if (check_if_in(MONSTER_HIGH_HP_TILES, frame.data[iy][ix].ch)) break;
            double intensity = pow(falloff, j);
            if (intensity < 0.1) continue;
            fov.insert(make_pair(ix, iy));
            pixel_intensities[make_pair(ix, iy)] = intensity;
            if (check_if_in(WALL_TILES, frame.data[iy][ix].ch)) break;
            x += dx, y += dy;
        }
    }
    fov.insert(make_pair(pos.x, pos.y));
    return fov;
}

Frame
LightSystem::addPointLight(Frame frame, GameMap &game_map, Position light_pos, int radius, char temperature,
                           int start_y, int start_x) {
    pair<int,int> frame_size = {frame.data.size(), frame.data[0].size()};
    Position light_on_frame_pos = {light_pos.x - start_x, light_pos.y - start_y};

    set<pair<int,int>> cast_clear = castLight(frame, light_on_frame_pos, max(1, radius / 3), 0.999999);
    set<pair<int,int>> cast_drop1 = castLight(frame, light_on_frame_pos, max(1, radius * 2 / 3), 0.999999);
    set<pair<int,int>> cast_drop2 = castLight(frame, light_on_frame_pos, max(1, radius * 3 / 4), 0.999999);
    set<pair<int,int>> cast_drop3 = castLight(frame, light_on_frame_pos, max(1, radius * 4 / 5), 0.999999);

    for (const pair<int,int> cast: cast_drop2) {
        if (cast_drop3.find(cast) != cast_drop3.end()) {
            cast_drop3.erase(cast);
        }
    }
    for (const pair<int,int> cast: cast_drop1) {
        if (cast_drop2.find(cast) != cast_drop2.end()) {
            cast_drop2.erase(cast);
        }
    }
    for (const pair<int,int> cast: cast_clear) {
        if (cast_drop1.find(cast) != cast_drop1.end()) {
            cast_drop1.erase(cast);
        }
    }


    vector<pair<short, set<pair<int,int>>>> falloff_palette = {
            make_pair(0, cast_clear),
            make_pair(1, cast_drop1),
            make_pair(2, cast_drop2),
            make_pair(3, cast_drop3)
    };

    for (const auto &palette: falloff_palette) {
        for (pair<int,int> coords: palette.second) {
            if (!(coords.second < 0 || coords.second >= frame_size.first || coords.first < 0 ||
                  coords.first >= frame_size.second)
                && frame.data[coords.second][coords.first].bg_color == NCOLOR_BLACK
                && (check_if_in(GROUND_TILES, frame.data[coords.second][coords.first].ch)
                    || check_if_in(WALL_TILES, frame.data[coords.second][coords.first].ch))
                    ) {
                frame.data[coords.second][coords.first].fg_color =
                        game_map.data[coords.first + start_x][coords.second + start_y].color;
                for (int bake = 1; bake <= palette.first; bake++) {
                    Color this_color;
                    Color bg_color = frame.data[coords.second][coords.first].bg_color;
                    Color sampled_color = frame.data[coords.second][coords.first].fg_color;
                    if (temperature == 'w') {
                        if (sampled_color == NCOLOR_YELLOW
                            || sampled_color == NCOLOR_ORANGE
                            || sampled_color == NCOLOR_RED) {
                            this_color = WARM_RED_LIGHT_FALLOFF_MAP[sampled_color];
                        } else if (sampled_color == NCOLOR_PINK
                                   || sampled_color == NCOLOR_PURPLE
                                   || sampled_color == NCOLOR_BLUE) {
                            this_color = WARM_BLUE_LIGHT_FALLOFF_MAP[sampled_color];

                        } else if (sampled_color == NCOLOR_LGREEN
                                   || sampled_color == NCOLOR_GREEN) {
                            this_color = WARM_GREEN_LIGHT_FALLOFF_MAP[sampled_color];

                        }

                    } else if (temperature == 'c') {
                        if (sampled_color == NCOLOR_AMARINE
                            || sampled_color == NCOLOR_LBLUE
                            || sampled_color == NCOLOR_BLUE) {
                            this_color = COLD_BLUE_LIGHT_FALLOFF_MAP[sampled_color];

                        } else if (sampled_color == NCOLOR_PINK
                                   || sampled_color == NCOLOR_MAGENTA
                                   || sampled_color == NCOLOR_RED) {
                            this_color = COLD_RED_LIGHT_FALLOFF_MAP[sampled_color];

                        } else if (sampled_color == NCOLOR_CYAN
                                   || sampled_color == NCOLOR_GREEN) {
                            this_color = COLD_GREEN_LIGHT_FALLOFF_MAP[sampled_color];

                        }

                    } else {
                        this_color = GREYSCALE_LIGHT_FALLOFF_MAP[sampled_color];
                    }
                    frame.data[coords.second][coords.first].fg_color = this_color;
                    frame.data[coords.second][coords.first].fg_color = bg_color;

                }
                //frame.data[coords.second][coords.first].second.first = palette.first;

            }
        }
    }

    return
            frame;
}

Frame
LightSystem::renderLighting(Frame frame, GameMap &game_map, Position player_pos, int player_light_radius, int start_y,
                            int start_x, int end_y, int end_x) {
    frame = addPointLight(frame, game_map, player_pos, player_light_radius, 'n', start_y, start_x);
    return frame;
}
