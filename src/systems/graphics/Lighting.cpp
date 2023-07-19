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


unordered_set<Position, PositionHash>
LightSystem::castLight(Position pos, int radius, int start_x, int start_y, int end_x, int end_y) {
    unordered_set<Position, PositionHash> fov;
    for (int i = 0; i < 360; ++i) {
        double rad = i * (M_PI / 180.0);
        double dx = cos(rad), dy = sin(rad);
        double x = pos.x, y = pos.y;
        for (int j = 0; j <= radius; ++j) {
            bool cull = false;

            int ix = round(x), iy = round(y);
            if (iy < 0 || iy >= currentEntityMap->data.size() || ix < 0 || ix >= currentEntityMap->data[0].size()
                || ix < start_x || ix >= end_x || iy < start_y || iy >= end_y)
                continue;

            if (!currentEntityMap->data[ix][iy].empty() && pos.x != ix && pos.y != iy) {
                // && currentEntityMap->data[ix][iy][0].id != 1) {
                for (const auto &e: currentEntityMap->data[ix][iy]) {
                    if (!e.transient) cull = true;
                    break;
                }
            }
            if (currentGameMap->data[ix][iy].z) cull = true;
            fov.insert({ix, iy});
            if (cull) {
                break;
            }
            x += dx, y += dy;
        }
    }
    fov.insert({pos.x, pos.y});
    return fov;
}


pair<Color, Color> LightSystem::degradeGrey(pair<Color, Color> &this_color) const {
    Color fg_color = {(uint8_t) ((double) this_color.first.red * 0.9),
                      (uint8_t) ((double) this_color.first.green * 0.9),
                      (uint8_t) ((double) this_color.first.blue * 0.9)};
    Color bg_color = {(uint8_t) ((double) this_color.second.red * 0.75),
                      (uint8_t) ((double) this_color.second.green * 0.75),
                      (uint8_t) ((double) this_color.second.blue * 0.75)};
    return make_pair(fg_color, bg_color);
}

pair<Color, Color> LightSystem::degradeWarm(pair<Color, Color> &this_color) const {
    Color fg_color = {(uint8_t) ((double) this_color.first.red * 0.95),
                      (uint8_t) ((double) this_color.first.green * 0.9),
                      (uint8_t) ((double) this_color.first.blue * 0.85)};
    Color bg_color = {(uint8_t) ((double) this_color.second.red * 0.85),
                      (uint8_t) ((double) this_color.second.green * 0.75),
                      (uint8_t) ((double) this_color.second.blue * 0.65)};
    return make_pair(fg_color, bg_color);
}

pair<Color, Color> LightSystem::degradeCold(pair<Color, Color> &this_color) const {
    Color fg_color = {(uint8_t) ((double) this_color.first.red * 0.85),
                      (uint8_t) ((double) this_color.first.green * 0.9),
                      (uint8_t) ((double) this_color.first.blue * 0.95)};
    Color bg_color = {(uint8_t) ((double) this_color.second.red * 0.65),
                      (uint8_t) ((double) this_color.second.green * 0.75),
                      (uint8_t) ((double) this_color.second.blue * 0.85)};
    return make_pair(fg_color, bg_color);
}


pair<Color, Color> LightSystem::degradeColorTemperature(pair<Color, Color> &this_color, char color_temp) const {
    if (color_temp == 'w') { //warm degrade
        this_color = degradeWarm(this_color);
    } else if (color_temp == 'c') { //cold degrade
        this_color = degradeCold(this_color);
    } else { //default neutral degrade
        this_color = degradeGrey(this_color);
    }

    return this_color;
}

uint8_t LightSystem::getFlickerVal(uint8_t val, double flicker) const {
    int s = 3;
    return (uint8_t) (max(0, min(255, get_random_int((double) val - s * flicker, (double) val + s * flicker))));
}

pair<Color, Color> LightSystem::flickerLight(pair<Color, Color> &this_color, const double flicker) const {
    Color fg_color = {getFlickerVal(this_color.first.red, flicker), getFlickerVal(this_color.first.green, flicker),
                      getFlickerVal(this_color.first.blue, flicker)};
    Color bg_color = {getFlickerVal(this_color.second.red, flicker), getFlickerVal(this_color.second.green, flicker),
                      getFlickerVal(this_color.second.blue, flicker)};
    return make_pair(fg_color, bg_color);
}

Frame LightSystem::addPointLight(Frame frame, Position light_pos, Light light, int start_y, int start_x) {
    pair<int, int> frame_size = {frame.data.size(), frame.data[0].size()};
    vector<unordered_set<Position, PositionHash>> cast_groups(10);
    int flicker_offset = 0;
    for (int i = 0; i < 10; ++i) {
        if (light.flicker > 0.0 && !get_random_int(0, (double) 255 * (1.0 - light.flicker))) {
            flicker_offset += get_random_int(-1, 1);
        }
        int light_radius =
                max((i + 1) * light.radius / 10, light.radius / 3 + i * light.radius / 10) + flicker_offset;
        cast_groups[i] = castLight(light_pos, light_radius, start_x, start_y, start_x + frame_size.second,
                                   start_y + frame_size.first);
    }
    for (int i = 9; i > 0; --i) {
        for (const Position &cast: cast_groups[i - 1]) {
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
        for (const Position &coords: palette.second) {
            Position frame_coords = {coords.x - start_x, coords.y - start_y};
            if (!(frame_coords.y < 0 || frame_coords.y >= frame_size.first || frame_coords.x < 0 ||
                  frame_coords.x >= frame_size.second)
                && frame.data[frame_coords.y][frame_coords.x].bg_color != NCOLOR_BLACK) {
                Color unlit_fg_color =
                        currentGameMap->data[frame_coords.x + start_x][frame_coords.y + start_y].fg_color;
                Color unlit_bg_color =
                        currentGameMap->data[frame_coords.x + start_x][frame_coords.y + start_y].bg_color;


                pair<Color, Color> lit_colors = {
                        unlit_fg_color,
//                        {
//                                (uint8_t) min(255,
//                                              unlit_fg_color.red
//                                              + (int) ((float) light.color.red * light.intensity/3)),
//                                (uint8_t) min(255,
//                                              unlit_fg_color.green
//                                              + (int) ((float) light.color.green * light.intensity/3)),
//                                (uint8_t) min(255,
//                                              unlit_fg_color.blue
//                                              + (int) ((float) light.color.blue * light.intensity/3))
//                        },
                        {
                                (uint8_t) min(255,
                                              unlit_bg_color.red
                                              + (int) ((float) light.color.red * light.intensity)),
                                (uint8_t) min(255,
                                              unlit_bg_color.green
                                              + (int) ((float) light.color.green * light.intensity)),
                                (uint8_t) min(255,
                                              unlit_bg_color.blue
                                              + (int) ((float) light.color.blue * light.intensity))
                        }
                };


                for (int bake = 0; bake <= palette.first; bake++) {

                    if (bake) {
                        lit_colors = degradeColorTemperature(lit_colors, light.temp);
                    }

                    if (light.flicker > 0.0 && !get_random_int(0, (double) 32 * (1.0 - light.flicker))) {
                        lit_colors = flickerLight(lit_colors, light.flicker);
                    }


                }

                const pair<Color, Color> frame_color = make_pair(
                        frame.data[frame_coords.y][frame_coords.x].fg_color,
                        frame.data[frame_coords.y][frame_coords.x].bg_color
                );

                lit_colors = make_pair(
                        Color{
                                (uint8_t) (
                                        min((int) light.color.red,
                                            (int) lit_colors.first.red + (int) frame_color.first.red)
                                ),
                                (uint8_t) (
                                        min((int) light.color.green,
                                            (int) lit_colors.first.green + (int) frame_color.first.green)
                                ),
                                (uint8_t) (
                                        min((int) light.color.blue,
                                            (int) lit_colors.first.blue + (int) frame_color.first.blue)
                                )
                        },
                        Color{
                                (uint8_t) (
                                        min((int) light.color.red,
                                            (int) lit_colors.second.red + (int) frame_color.second.red)
                                ),
                                (uint8_t) (
                                        min((int) light.color.green,
                                            (int) lit_colors.second.green + (int) frame_color.second.green)
                                ),
                                (uint8_t) (
                                        min((int) light.color.blue,
                                            (int) lit_colors.second.blue + (int) frame_color.second.blue)
                                )
                        }
                );

                if (!currentGameMap->data[frame_coords.x + start_x][frame_coords.y + start_y].z) {
                    frame.data[frame_coords.y][frame_coords.x].fg_color = lit_colors.second;
                    lit_colors.first &= frame.data[frame_coords.y][frame_coords.x].bg_color;
                    lit_colors.first &= frame.data[frame_coords.y][frame_coords.x].bg_color;
                    lit_colors.first &= frame.data[frame_coords.y][frame_coords.x].bg_color;
                    frame.data[frame_coords.y][frame_coords.x].bg_color &= lit_colors.first;
                } else {
                    frame.data[frame_coords.y][frame_coords.x].fg_color *= lit_colors.first;
                    frame.data[frame_coords.y][frame_coords.x].bg_color &= lit_colors.second;
                }

            }
        }
    }

    return
            frame;
}

Frame LightSystem::addAmbientLight(Frame frame, Position light_pos, Color color, int start_y, int start_x) {
    pair<int, int> frame_size = {frame.data.size(), frame.data[0].size()};
    int cast_size = static_cast<int>(frame_size.second) / 2;
    for (const auto &light_pos: castLight(light_pos, cast_size, start_x, start_y, start_x + frame_size.second,
                                          start_y + frame_size.first)) {
        Position frame_position = {light_pos.x - start_x, light_pos.y - start_y};
        if (frame_position.y < 0 || frame_position.y >= frame.data.size()) continue;
        if (frame_position.x < 0 || frame_position.x >= frame.data[0].size()) continue;
        Color old_color = frame.data[light_pos.y - start_y][light_pos.x - start_x].bg_color;
        Color new_color = color;
        new_color += old_color;
        frame.data[light_pos.y - start_y][light_pos.x - start_x].bg_color = new_color;
    }
    return frame;
}

Frame
LightSystem::renderLighting2D(Frame frame, Position player_pos, int player_light_radius, int start_y, int start_x,
                              int end_y, int end_x) {
    Light player_light = {{250, 181, 82}, 'w', static_cast<unsigned short>(player_light_radius), 0.88, 0.7, 0.7};
    Color ambient_color = {1, 1, 1};


    frame = addAmbientLight(frame, player_pos, ambient_color, start_y, start_x);

    for (int i = start_y; i < end_y; ++i) {
        for (int j = start_x; j < end_x; ++j) {
            if (currentGameMap->data[j][i].emissive) {
                frame = addPointLight(frame, {j, i}, emissiveLight, start_y, start_x);
            }
        }
    }

    frame = addPointLight(frame, player_pos, player_light, start_y, start_x);


    return frame;
}