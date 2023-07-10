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


void LightSystem::setMaps(const GameMap &game_map, const EntityMap &entity_map, unsigned long x, unsigned long y) {
    currentGameMap = game_map;
    currentEntityMap = entity_map;


    staticMap = {
            currentGameMap.meta,
            vector<vector<Light>>(x, vector<Light>(y, noLight))
    };
    dynamicMap = {
            currentEntityMap.meta,
            vector<vector<Light>>(x, vector<Light>(y, noLight))
    };
}


void LightSystem::populateMaps() {
    staticMap.meta = {currentGameMap.meta};
    dynamicMap.meta = {currentEntityMap.meta};

    for (int i = 0; i < currentGameMap.data.size(); ++i) {
        for (int j = 0; j < currentGameMap.data[0].size(); ++j) {
            if (!currentGameMap.data[i][j].subterranean) {
                staticMap.data[i][j] = overworldLight;
            } else {
                staticMap.data[i][j] = underworldLight;
            }
            if (currentGameMap.data[i][j].emissive) {
                dynamicMap.data[i][j] = emissiveLight;

            }
        }
    }

}


void LightSystem::CleanUp() {
    // Clean up systems
}

void LightSystem::Update(const EntityMap &entity_map) {
    currentEntityMap = entity_map;

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
    return make_pair(this_color, bg_color);
}

pair<Color, Color> LightSystem::degradeWarm(Color &this_color, Color &bg_color) const {
    this_color = {
            (uint8_t) ((double) this_color.red * 0.75),
            (uint8_t) ((double) this_color.green * 0.5),
            (uint8_t) ((double) this_color.blue * 0.25)
    };
    bg_color = {
            (uint8_t) ((double) bg_color.red * 0.9),
            (uint8_t) ((double) bg_color.green * 0.75),
            (uint8_t) ((double) bg_color.blue * 0.5)
    };
    return make_pair(this_color, bg_color);
}

set<pair<int, int>> LightSystem::castLight(
        Position pos,
        int radius) {
    set<pair<int, int>> fov;
    vector<pair<double, double>> offsets = {{0.5,  0.5},
                                            {-0.5, 0.5},
                                            {0.5,  -0.5},
                                            {-0.5, -0.5}};
    for (int i = 0; i < 360; ++i) {
        double rad = i * (M_PI / 180.0);
        for (auto &offset: offsets) {
            double dx = cos(rad), dy = sin(rad) / 2;
            double x = pos.x + offset.first, y = pos.y + offset.second;
            for (int j = 0; j <= radius; ++j) {
                int ix = round(x), iy = round(y);
                if (max(0, (int) currentEntityMap.data[ix][iy].size() - 1) &&
                    currentEntityMap.data[ix][iy][0].id != 1)
                    break;
                fov.insert(make_pair(ix, iy));
                if (currentGameMap.data[ix][iy].z) break;
                x += dx, y += dy;
            }
        }
    }
    fov.insert(make_pair(pos.x, pos.y));
    return fov;
}


Frame LightSystem::addPointLight(Frame frame, Position light_pos, int radius, char temperature,
                                 int start_y, int start_x) {
    pair<int, int> frame_size = {frame.data.size(), frame.data[0].size()};

    vector<set<pair<int, int>>> cast_groups(10);
    for (int i = 0; i < 10; ++i) {
        cast_groups[i] = castLight(light_pos, max(i + 1, radius / 3 + i * radius / 10));
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
            Position frame_coords = {coords.first - start_x, coords.second - start_y};
            if (!(frame_coords.y < 0 || frame_coords.y >= frame_size.first || frame_coords.x < 0 ||
                  frame_coords.x >= frame_size.second)
                && frame.data[frame_coords.y][frame_coords.x].bg_color == NCOLOR_BLACK) {

                frame.data[frame_coords.y][frame_coords.x].fg_color =
                        currentGameMap.data[frame_coords.x + start_x][frame_coords.y + start_y].color;

                Color bg_color = {255, 227, 112};//light color, ig
                Color this_color = frame.data[frame_coords.y][frame_coords.x].fg_color;
                pair<Color, Color> new_color;
                for (int bake = 0; bake <= palette.first; bake++) {

                    // Moved color calculation to a helper function

                    // Simplified the color calculation code with a helper function
                    if (temperature == 'w') {
                        new_color = degradeWarm(this_color, bg_color);
                        //this_color = WARM_LIGHT_FALLOFF_MAP[sampledColor];
                    } else if (temperature == 'c') {
                        //this_color = COLD_LIGHT_FALLOFF_MAP[sampledColor];
                    } else {
                        new_color = degradeGreyscale(this_color, bg_color);
                    }

                    frame.data[frame_coords.y][frame_coords.x].fg_color = new_color.first;
                    frame.data[frame_coords.y][frame_coords.x].bg_color = new_color.second;
                }
            }
        }
    }

    return frame;
}


Frame
LightSystem::addWashLight(Frame frame, Position light_pos, Color color, int start_y, int start_x) {
    int cast_size = (int) frame.data[0].size() / 2;

    for (auto &light_pos: castLight(light_pos, cast_size)) {
        if (currentGameMap.data[light_pos.first][light_pos.second].z == 0) {
            Position frame_position = {light_pos.first - start_x, light_pos.second - start_y};
            if (frame_position.y < 0 || frame_position.y >= frame.data.size()) continue;
            if (frame_position.x < 0 || frame_position.x >= frame.data[0].size()) continue;
            Color old_color = frame.data[light_pos.second - start_y][light_pos.first - start_x].bg_color;
            uint8_t new_r = min(255, old_color.red + color.red);
            uint8_t new_g = min(255, old_color.green + color.green);
            uint8_t new_b = min(255, old_color.blue + color.blue);
            Color new_color = {new_r, new_g, new_b};
            frame.data[light_pos.second - start_y][light_pos.first - start_x].bg_color = new_color;
        }

    }

    return frame;
}


Frame
LightSystem::renderLighting(Frame frame, Position player_pos, int player_light_radius,
                            int start_y, int start_x, int end_y, int end_x) {
    frame = addPointLight(frame, player_pos, player_light_radius, 'w', start_y, start_x);
    frame = addWashLight(frame, player_pos, {25, 12, 12}, start_y, start_x);
    return frame;
}
