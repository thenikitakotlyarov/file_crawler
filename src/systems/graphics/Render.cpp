#include "Render.h"


RenderSystem::RenderSystem() {
    bool init_out = Initialize();
    if (!init_out) {
        running = false;
    } else {
        running = true;
    }

}

RenderSystem::~RenderSystem() {
    CleanUp();
}

bool RenderSystem::Initialize() {
    // Initialize systems
    return true;
}


void RenderSystem::CleanUp() {
    // Clean up systems
}

void RenderSystem::Update() {
    // Update systems
}
Sprite RenderSystem::charToSprite(EntitySystem& entitySystem, const Position pos,
                                  const wstring this_char, const Color fg_color, const Color bg_color) {
    Sprite this_sprite;

    const auto& entityMap = entitySystem.getEntities();
    const auto& monsters = entitySystem.getMonsters();
    const auto& items = entitySystem.getItems();
    const auto& player = entitySystem.getCurrentPlayer();

    if (this_char == L"@") {
        this_sprite = player.sprite;
    } else if (MONSTER_TILES.count(this_char)) {
        for (const auto& entity_ref : entityMap->data[pos.x][pos.y]) {
            if (monsters.count(entity_ref)) {
                this_sprite = monsters.at(entity_ref).sprite;
                break;
            }
        }
    } else if (ITEM_TILES.count(this_char)) {
        for (const auto& entity_ref : entityMap->data[pos.x][pos.y]) {
            if (items.count(entity_ref)) {
                this_sprite = items.at(entity_ref).sprite;
                break;
            }
        }
    } else {
        const auto it = upres_map.find(this_char);
        const vector<vector<wstring>>& upscaled_texture = (it != upres_map.end()) ? it->second : upres_map.at(L"?");
        this_sprite = {
                upscaled_texture,
                vector<vector<Color>>(5, vector<Color>(5, fg_color)),
                vector<vector<Color>>(5, vector<Color>(5, bg_color))
        };
    }

    return this_sprite;
}


Frame RenderSystem::ppUpscale(const Frame &frame, EntitySystem &entitySystem,
                              const int start_x, const int start_y, const unsigned short scale) {
    //ups the resolution of a frame to a pixel resolution of input:output::1:scale
    Position frame_size = {
            (int) frame.data[0].size(),
            (int) frame.data.size()
    };

    Frame new_frame = {
            frame.meta,
            vector<vector<Pixel>>(frame_size.y * scale,
                                  vector<Pixel>(frame_size.x * scale,
                                                Pixel{L"?", NCOLOR_RED, NCOLOR_BLACK}
                                  )
            )
    };


    for (int i = 0; i < frame_size.y; ++i) {
        for (int j = 0; j < frame_size.x; ++j) {
            const wstring this_char = frame.data[i][j].ch;
            const Sprite this_sprite = charToSprite(entitySystem, {j + start_x, i + start_y},
                                                    this_char, frame.data[i][j].fg_color, frame.data[i][j].bg_color);


            for (int _i = 0; _i < scale; ++_i) {
                for (int _j = 0; _j < scale; ++_j) {
                    Position this_pos = {j * scale + _j,
                                         i * scale + _i};
                    new_frame.data[this_pos.y][this_pos.x] = frame.data[i][j];
                    new_frame.data[this_pos.y][this_pos.x].ch = this_sprite.texture[_i][_j];
                    new_frame.data[this_pos.y][this_pos.x].fg_color = this_sprite.fg_colors[_i][_j];
                    new_frame.data[this_pos.y][this_pos.x].bg_color = this_sprite.bg_colors[_i][_j];


                    Color old_subpixel_color = new_frame.data[i][j].bg_color;
                    if (this_sprite.texture[_i][_j] == L" ") {
                        Color new_subpixel_color;
                        Position offset = {0, 0};
                        bool stop = false;
                        int depth = 0;
                        do {
                            if (_i < scale / 3) {
                                offset.y -= 1;
                            } else if (_i > scale * 2 / 3) {
                                offset.y += 1;
                            }
                            if (_j < scale / 3) {
                                offset.x -= 1;
                            } else if (_j > scale * 2 / 3) {
                                offset.x += 1;
                            }

                            Position new_pos = {
                                    max(0, min(frame_size.x - 1, j + offset.x)),
                                    max(0, min(frame_size.y - 1, i + offset.y))
                            };

                            new_subpixel_color = frame.data[new_pos.y][new_pos.x].bg_color;


                            if (new_subpixel_color != old_subpixel_color) stop = true;
                            depth++;
                        } while (!stop && !depth);


                        if (new_subpixel_color < new_frame.data[this_pos.y][this_pos.x].bg_color) {
                            new_frame.data[this_pos.y][this_pos.x].bg_color = new_subpixel_color;
                        }

                    }


                }

            }

        }
    }

    return new_frame;
}

Frame RenderSystem::ppBlurLight(const Frame &frame, const unsigned short scale, const float amount) {

    const unsigned short kernel_scale = 1 + 2 * scale;

    Position frame_size = {(int) frame.data[0].size(), (int) frame.data.size()};

    Frame new_frame = frame;

    for (int i = 0; i < frame_size.y; ++i) {
        for (int j = 0; j < frame_size.x; ++j) {
            Color this_fg_color = {0, 0, 0};
            Color this_bg_color = {0, 0, 0};
            Color sample_color;
            for (int _i = 0; _i < kernel_scale; ++_i) {
                for (int _j = 0; _j < kernel_scale; ++_j) {
                    Position offset = {_j - scale, _i - scale};
                    Position frame_position = {
                            max(0, min(frame_size.x - 1, j + offset.x)),
                            max(0, min(frame_size.y - 1, i + offset.y)),
                    };
                    sample_color = frame.data[frame_position.y][frame_position.x].bg_color;
                    //float weight = 1/16;
                    float weight = 1 / pow(2, (abs(offset.x) + abs(offset.y) + 2)) / pow(1.5 - pow(2, -scale), 2);

                    this_fg_color = {
                            (uint8_t) ((float) this_fg_color.red + (float) sample_color.red * weight / 2 * amount),
                            (uint8_t) ((float) this_fg_color.green + (float) sample_color.green * weight / 2 * amount),
                            (uint8_t) ((float) this_fg_color.blue + (float) sample_color.blue * weight / 2 * amount),
                    };

                    this_bg_color = {
                            (uint8_t) ((float) this_fg_color.red + (float) sample_color.red * weight * amount),
                            (uint8_t) ((float) this_fg_color.green + (float) sample_color.green * weight * amount),
                            (uint8_t) ((float) this_fg_color.blue + (float) sample_color.blue * weight * amount),
                    };


                };
            }
            //new_frame.data[i][j].fg_color &= this_fg_color;
            new_frame.data[i][j].fg_color &= this_fg_color;
            new_frame.data[i][j].bg_color += this_bg_color;
        }
    }


    return
            new_frame;
}

// Render frame on screen
void RenderSystem::render(const Frame& frame) {
    const int frameHeight = frame.data.size();
    const int frameWidth = frame.data[0].size();

    // Construct the output string
    wstring output;
    for (int i = 0; i < frameHeight; ++i) {
        for (int j = 0; j < frameWidth; ++j) {
            const Pixel& pixel = frame.data[i][j];
            const wstring& pixel_char = pixel.ch;
            const uint8_t& fg_r = pixel.fg_color.red;
            const uint8_t& fg_g = pixel.fg_color.green;
            const uint8_t& fg_b = pixel.fg_color.blue;
            const uint8_t& bg_r = pixel.bg_color.red;
            const uint8_t& bg_g = pixel.bg_color.green;
            const uint8_t& bg_b = pixel.bg_color.blue;

            for (const wchar_t& wc : pixel_char) {
                output += L"\033[" + to_wstring(i + 1) + L";" + to_wstring(j + 1) +
                          L"H\033[38;2;" + to_wstring(fg_r) + L";" + to_wstring(fg_g) + L";" +
                          to_wstring(fg_b) + L";48;2;" + to_wstring(bg_r) + L";" +
                          to_wstring(bg_g) + L";" + to_wstring(bg_b) + L"m" + wc;
            }
        }
    }

    // Print the output string once
    wprintf(L"%ls", output.c_str());
    fflush(stdout);
}
