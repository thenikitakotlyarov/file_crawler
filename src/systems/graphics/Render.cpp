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

//ups the resolution of a frame to a pixel resolution of input:output::1:n
Frame RenderSystem::upRes(const Frame &frame, const unsigned short n) {
    Position frame_size = {
            (int) frame.data[0].size(),
            (int) frame.data.size()
    };

    Frame new_frame = {
            frame.meta,
            vector<vector<Pixel>>(frame_size.y * n,
                                  vector<Pixel>(frame_size.x * n,
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

            for (int _i = 0; _i < n; ++_i) {
                for (int _j = 0; _j < n; ++_j) {
                    Position this_pos = {j * n + _j, i * n + _i};
                    new_frame.data[this_pos.y][this_pos.x] = frame.data[i][j];
                    new_frame.data[this_pos.y][this_pos.x].ch = this_sprite[_i][_j];
                    Color old_subpixel_color = frame.data[i][j].bg_color;
                    if (this_sprite[_i][_j] == L" ") {
                        Color new_subpixel_color;
                        Position offset = {0, 0};
                        bool stop = false;
                        int depth = 0;
                        do {
                            if (_i < n / 3) {
                                offset.y -= 1;
                            } else if (_i > n * 2 / 3) {
                                offset.y += 1;
                            }

                            if (_j < n / 3) {
                                offset.x -= 1;
                            } else if (_j > n * 2 / 3) {
                                offset.x += 1;
                            }

                            if (i + offset.y < 0 || i + offset.y >= frame.data.size()
                                || j + offset.x < 0 || j + offset.x >= frame.data[0].size()) {
                                stop = true;
                            } else {
                                new_subpixel_color = frame.data[i + offset.y][j + offset.x].bg_color;

                            }
                            if (new_subpixel_color != old_subpixel_color) stop = true;
                            depth++;
                        } while (!stop && !depth);

                        new_frame.data[this_pos.y][this_pos.x].bg_color = new_subpixel_color;

                    }


                }

            }

        }
    }

    return new_frame;
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
