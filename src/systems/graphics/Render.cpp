#pragma once

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
