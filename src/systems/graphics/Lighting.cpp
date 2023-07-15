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


void LightSystem::setMaps(GameMap *game_map, EntityMap *entity_map, unsigned long x, unsigned long y) {
    currentGameMap = game_map;
    currentEntityMap = entity_map;


    staticMap = {
            currentGameMap->meta,
            vector<vector<Light>>(x, vector<Light>(y, noLight))
    };
    dynamicMap = {
            currentEntityMap->meta,
            vector<vector<Light>>(x, vector<Light>(y, noLight))
    };
}


void LightSystem::populateMaps() {

    for (int i = 0; i < currentGameMap->data.size(); ++i) {
        for (int j = 0; j < currentGameMap->data[0].size(); ++j) {
            if (!currentGameMap->data[i][j].subterranean) {
                staticMap.data[i][j] = overworldLight;
            } else {
                staticMap.data[i][j] = underworldLight;
            }
            if (currentGameMap->data[i][j].emissive) {
                dynamicMap.data[i][j] = emissiveLight;

            }
        }
    }

}


void LightSystem::CleanUp() {
    // Clean up systems
}

void LightSystem::Update(EntityMap *entity_map) {
    currentEntityMap = entity_map;

}


pair<Color, Color> LightSystem::degradeGreyscale(pair<Color, Color> &this_color) const {
    Color fg_color = {
            (uint8_t) ((double) this_color.first.red * 0.9),
            (uint8_t) ((double) this_color.first.green * 0.9),
            (uint8_t) ((double) this_color.first.blue * 0.9)
    };
    Color bg_color = {
            (uint8_t) ((double) this_color.second.red * 0.75),
            (uint8_t) ((double) this_color.second.green * 0.75),
            (uint8_t) ((double) this_color.second.blue * 0.75)
    };
    return make_pair(fg_color, bg_color);
}

pair<Color, Color> LightSystem::degradeWarm(pair<Color, Color> &this_color) const {
    Color fg_color = {
            (uint8_t) ((double) this_color.first.red * 0.95),
            (uint8_t) ((double) this_color.first.green * 0.9),
            (uint8_t) ((double) this_color.first.blue * 0.85)
    };
    Color bg_color = {
            (uint8_t) ((double) this_color.second.red * 0.85),
            (uint8_t) ((double) this_color.second.green * 0.75),
            (uint8_t) ((double) this_color.second.blue * 0.65)
    };
    return make_pair(fg_color, bg_color);
}

pair<Color, Color> LightSystem::degradeCold(pair<Color, Color> &this_color) const {
    Color fg_color = {
            (uint8_t) ((double) this_color.first.red * 0.85),
            (uint8_t) ((double) this_color.first.green * 0.9),
            (uint8_t) ((double) this_color.first.blue * 0.95)
    };
    Color bg_color = {
            (uint8_t) ((double) this_color.second.red * 0.65),
            (uint8_t) ((double) this_color.second.green * 0.75),
            (uint8_t) ((double) this_color.second.blue * 0.85)
    };
    return make_pair(fg_color, bg_color);
}

unordered_set<Position, PositionHash> LightSystem::castLight(
        Position pos,
        int radius,
        int start_x, int start_y, int end_x, int end_y) {
    unordered_set<Position, PositionHash> fov;
//    vector<pair<double, double>> offsets = {{0.5,  0.5},
//                                            {-0.5, 0.5},
//                                            {0.5,  -0.5},
//                                            {-0.5, -0.5}};
    for (int i = 0; i < 360; ++i) {
        double rad = i * (M_PI / 180.0);
        //for (auto &offset: offsets) {
        double dx = cos(rad), dy = sin(rad) / 2;
        double x = pos.x, y = pos.y;
        //double x = pos.x + offset.first, y = pos.y + offset.second;
        for (int j = 0; j <= radius; ++j) {
            int ix = round(x), iy = round(y);
            if (ix < start_x || ix >= end_x || iy < start_y || iy >= end_y) continue;
            if (iy < 0 || iy >= currentEntityMap->data.size()
                || ix < 0 || ix >= currentEntityMap->data[0].size())
                continue;
            bool cull = false;
            if (!currentEntityMap->data[ix][iy].empty()
                && currentEntityMap->data[ix][iy][0].id != 1) {
                for (const auto &e: currentEntityMap->data[ix][iy]) {
                    if (!e.transient) cull = true;
                    break;
                }
            }
            fov.insert({ix, iy});
            if (currentGameMap->data[ix][iy].z) break;
            if (cull) {
                //int I_AM_A_DEBUG = break_point();
                break;
            }
            x += dx, y += dy;
        }
        //}
    }
    fov.insert({pos.x, pos.y});
    return fov;
}


Frame LightSystem::addPointLight(Frame frame, Position light_pos, Light light,
                                 int start_y, int start_x) {
    pair<int, int> frame_size = {frame.data.size(), frame.data[0].size()};

    vector<unordered_set<Position, PositionHash>> cast_groups(10);
    for (int i = 0; i < 10; ++i) {
        cast_groups[i] = castLight(light_pos, max(i + 1, light.radius / 3 + i * light.radius / 10),
                                   start_x, start_y,
                                   start_x + frame_size.second, start_y + frame_size.first);
    }

    for (int i = 9; i > 0; --i) {
        for (const Position cast: cast_groups[i - 1]) {
            if (cast_groups[i].find(cast) != cast_groups[i].end()) {
                cast_groups[i].erase(cast);
            }
        }
    }

    vector<pair<short, unordered_set<Position, PositionHash>>> falloff_palette(10);
    for (int i = 0; i < 10; ++i) {
        falloff_palette[i] = {i, cast_groups[i]};
    }

    for (const auto &palette: falloff_palette) {
        for (Position coords: palette.second) {
            Position frame_coords = {coords.x - start_x, coords.y - start_y};
            if (!(frame_coords.y < 0 || frame_coords.y >= frame_size.first || frame_coords.x < 0 ||
                  frame_coords.x >= frame_size.second)
                && frame.data[frame_coords.y][frame_coords.x].bg_color == NCOLOR_BLACK) {
//
//                frame.data[frame_coords.y][frame_coords.x].fg_color =
//                        currentGameMap->data[frame_coords.x + start_x][frame_coords.y + start_y].fg_color;


                const Color &unlit_fg_color =
                        currentGameMap->data[frame_coords.x + start_x][frame_coords.y + start_y].fg_color;
                const Color &unlit_bg_color =
                        currentGameMap->data[frame_coords.x + start_x][frame_coords.y + start_y].bg_color;


                pair<Color, Color> lit_colors = {
                        {
                                (uint8_t) (
                                        (float) unlit_fg_color.red * light.intensity
                                        + (float) light.color.red * (1.0 - light.intensity)
                                ),
                                (uint8_t) (
                                        (float) unlit_fg_color.green * light.intensity
                                        + (float) light.color.green * (1.0 - light.intensity)
                                ),
                                (uint8_t) (
                                        (float) unlit_fg_color.blue * light.intensity
                                        + (float) light.color.blue * (1.0 - light.intensity)
                                )
                        },
                        {
                                (uint8_t) (
                                        (float) unlit_bg_color.red * (1.0 - light.intensity)
                                        + (float) light.color.red * light.intensity
                                ),
                                (uint8_t) (
                                        (float) unlit_bg_color.green * (1.0 - light.intensity)
                                        + (float) light.color.green * light.intensity
                                ),
                                (uint8_t) (
                                        (float) unlit_bg_color.blue * (1.0 - light.intensity)
                                        + (float) light.color.blue * light.intensity
                                )
                        }
                };


                for (int bake = 0; bake <= palette.first; bake++) {

                    // Moved color calculation to a helper function

                    // Simplified the color calculation code with a helper function
                    if (light.temp == 'w') {
                        lit_colors = degradeWarm(lit_colors);
                        //this_color = WARM_LIGHT_FALLOFF_MAP[sampledColor];
                    } else if (light.temp == 'c') {
                        lit_colors = degradeCold(lit_colors);
                        //this_color = COLD_LIGHT_FALLOFF_MAP[sampledColor];
                    } else {
                        lit_colors = degradeGreyscale(lit_colors);
                    }

                    if (!currentGameMap->data[frame_coords.x + start_x][frame_coords.y + start_y].z) {
                        frame.data[frame_coords.y][frame_coords.x].fg_color = lit_colors.first;
                        frame.data[frame_coords.y][frame_coords.x].bg_color = lit_colors.second;

                    } else {
                        frame.data[frame_coords.y][frame_coords.x].fg_color = lit_colors.second;
                        frame.data[frame_coords.y][frame_coords.x].bg_color = lit_colors.first;

                    }
                }
            }
        }
    }

    return frame;
}


Frame
LightSystem::addWashLight(Frame frame, Position light_pos, Color color, int start_y, int start_x) {
    pair<int, int> frame_size = {frame.data.size(), frame.data[0].size()};

    int cast_size = (int) frame_size.second / 2;

    for (auto &light_pos: castLight(light_pos, cast_size,
                                    start_x, start_y, start_x + frame_size.second, start_y + frame_size.first)) {
        if (currentGameMap->data[light_pos.x][light_pos.y].z == 0) {
            Position frame_position = {light_pos.x - start_x, light_pos.y - start_y};
            if (frame_position.y < 0 || frame_position.y >= frame.data.size()) continue;
            if (frame_position.x < 0 || frame_position.x >= frame.data[0].size()) continue;
            Color old_color = frame.data[light_pos.y - start_y][light_pos.x - start_x].bg_color;
            uint8_t new_r = min(255, old_color.red + color.red);
            uint8_t new_g = min(255, old_color.green + color.green);
            uint8_t new_b = min(255, old_color.blue + color.blue);
            Color new_color = {new_r, new_g, new_b};
            frame.data[light_pos.y - start_y][light_pos.x - start_x].bg_color = new_color;
        }

    }

    return frame;
}


Frame
LightSystem::renderLighting(Frame frame, Position player_pos, int player_light_radius,
                            int start_y, int start_x, int end_y, int end_x) {
    Light player_light = {
            {255, 168, 62}, 'w', (unsigned short) player_light_radius,
            0.88, 0.7, 0.7
    };

    Color wash_color = {24, 16, 20};

    frame = addPointLight(frame, player_pos, player_light, start_y, start_x);
    frame = addWashLight(frame, player_pos, wash_color, start_y, start_x);
    return frame;
}
