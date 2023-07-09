#include "Lighting.h"


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

set<pair<int, int>> LightSystem::castLight(
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
//            double intensity = pow(falloff, j);
//            if (intensity < 0.1) continue;
            fov.insert(make_pair(ix, iy));
//            pixel_intensities[make_pair(ix, iy)] = intensity;
            if (check_if_in(WALL_TILES, frame.data[iy][ix].ch)) break;
            x += dx, y += dy;
        }
    }
    fov.insert(make_pair(pos.x, pos.y));
    return fov;
}

Frame LightSystem::addPointLight(Frame frame, GameMap &game_map, Position light_pos, int radius, char temperature,
                                 int start_y, int start_x) {
    pair<int, int> frame_size = {frame.data.size(), frame.data[0].size()};
    Position light_on_frame_pos = {light_pos.x - start_x, light_pos.y - start_y};

    vector<set<pair<int, int>>> cast_groups(10);
    for (int i = 0; i < 10; ++i) {
        cast_groups[i] = castLight(frame, light_on_frame_pos, max(i + 1, radius / 3 + i * radius / 10), pow(0.999, i));
    }

    for (int i = 9; i > 0; --i) {
        for (const pair<int, int> cast: cast_groups[i - 1]) {
            if (cast_groups[i].find(cast) != cast_groups[i].end()) {
                cast_groups[i].erase(cast);
            }
        }
    }

    vector<pair<short, set<pair<int, int>>>> falloff_palette(10);
    for (int i = 0; i < 10; ++i) {
        falloff_palette[i] = make_pair(i, cast_groups[i]);
    }

    for (const auto &palette: falloff_palette) {
        for (pair<int, int> coords: palette.second) {
            if (!(coords.second < 0 || coords.second >= frame_size.first || coords.first < 0 ||
                  coords.first >= frame_size.second)
                && frame.data[coords.second][coords.first].bg_color == NCOLOR_BLACK
                && (check_if_in(GROUND_TILES, frame.data[coords.second][coords.first].ch)
                    || check_if_in(WALL_TILES, frame.data[coords.second][coords.first].ch))) {

                frame.data[coords.second][coords.first].fg_color =
                        game_map.data[coords.first + start_x][coords.second + start_y].color;

                Color bg_color = NCOLOR_WHITE;
                Color this_color = frame.data[coords.second][coords.first].fg_color;
                pair<Color,Color> new_color;
                for (int bake = 0; bake <= palette.first; bake++) {

                    // Moved color calculation to a helper function

                    // Simplified the color calculation code with a helper function
                    if (temperature == 'w') {
                        //this_color = WARM_LIGHT_FALLOFF_MAP[sampledColor];
                    } else if (temperature == 'c') {
                        //this_color = COLD_LIGHT_FALLOFF_MAP[sampledColor];
                    } else {
                        new_color = degradeGreyscale(bg_color, this_color);
                    }

                    frame.data[coords.second][coords.first].fg_color = new_color.first;
                    frame.data[coords.second][coords.first].bg_color = new_color.second;
                }
            }
        }
    }

    return frame;
}

pair<Color, Color> LightSystem::degradeGreyscale(Color &this_color, Color &bg_color) const {
    this_color = {
            (uint8_t) ((double) this_color.red * 0.85),
            (uint8_t) ((double) this_color.green * 0.85),
            (uint8_t) ((double) this_color.blue * 0.85)
    };
    bg_color = {
            (uint8_t) ((double) bg_color.red * 0.75),
            (uint8_t) ((double) bg_color.green * 0.75),
            (uint8_t) ((double) bg_color.blue * 0.75)
    };
    return make_pair(this_color,bg_color);
}


Frame
LightSystem::renderLighting(Frame frame, GameMap &game_map, Position player_pos, int player_light_radius, int start_y,
                            int start_x, int end_y, int end_x) {
    frame = addPointLight(frame, game_map, player_pos, player_light_radius, 'n', start_y, start_x);
    return frame;
}
