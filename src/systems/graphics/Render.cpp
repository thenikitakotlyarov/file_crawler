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

Frame RenderSystem::ppUpscale(const Frame &frame, const unsigned short scale) {
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
            wstring this_char = frame.data[i][j].ch;
            vector<vector<wstring>> this_sprite = upres_map[this_char];

            if (this_sprite.empty()) {
                this_sprite = upres_map[L"?"];
            }

            for (int _i = 0; _i < scale; ++_i) {
                for (int _j = 0; _j < scale; ++_j) {
                    Position this_pos = {j * scale + _j,
                                         i * scale + _i};
                    new_frame.data[this_pos.y][this_pos.x] = frame.data[i][j];
                    new_frame.data[this_pos.y][this_pos.x].ch = this_sprite[_i][_j];
                    Color old_subpixel_color = frame.data[i][j].bg_color;
                    if (this_sprite[_i][_j] == L" ") {
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
                                    max(0, min(frame_size.x-1, j + offset.x)),
                                    max(0, min(frame_size.y-1, i + offset.y))
                            };

                            new_subpixel_color = frame.data[new_pos.y][new_pos.x].bg_color;


                            if (new_subpixel_color != old_subpixel_color) stop = true;
                            depth++;
                        } while (!stop && !depth);


                        if (new_subpixel_color > new_frame.data[this_pos.y][this_pos.x].bg_color) {
                            new_frame.data[this_pos.y][this_pos.x].bg_color &= new_subpixel_color;
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
                    //float weight = 1 / pow(2, (abs(offset.x) + abs(offset.y) + 2)) / pow(1.5 - pow(2, -scale), 2);
                    float temp = pow(2, scale);
                    float weight = 1 / (4 * pow(2, abs(offset.x)) * pow(2, abs(offset.y)) * temp * temp);

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
void RenderSystem::render(const Frame &frame) {

    //clear();

    for (int i = 0; i < frame.data.size(); ++i) {
        for (int j = 0; j < frame.data[0].size(); ++j) {
            const wstring &pixel_char = frame.data[i][j].ch;
            const uint8_t &fg_r = frame.data[i][j].fg_color.red;
            const uint8_t &fg_g = frame.data[i][j].fg_color.green;
            const uint8_t &fg_b = frame.data[i][j].fg_color.blue;
            const uint8_t &bg_r = frame.data[i][j].bg_color.red;
            const uint8_t &bg_g = frame.data[i][j].bg_color.green;
            const uint8_t &bg_b = frame.data[i][j].bg_color.blue;

//            wchar_t pixel_ch[256];
//            swprintf(pixel_ch, 256, L"%s",
//                     pixel_char);

            for (const wchar_t &wc: pixel_char) {
                wprintf(L"\033[%d;%dH\033[38;2;%d;%d;%d;48;2;%d;%d;%dm%lc",
                        i + 1, j + 1,
                        fg_r, fg_g, fg_b,
                        bg_r, bg_g, bg_b,
                        wc);
                //fflush(stdout);
            }

        }
    }



    //wnoutrefresh(stdscr);
    //doupdate();
    fflush(stdout);
}
