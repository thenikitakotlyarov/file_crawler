#pragma once

#include "systems/entity/Entity.h"
#include "helpers.h"
#include "constants.h"

using namespace std;


class RenderSystem {
public:
    RenderSystem();

    ~RenderSystem();

    int running;

    void CleanUp();

    void Update();

    Sprite
    charToSprite(EntitySystem &entitySystem, const Position pos, const unsigned short scale,
                 const wstring this_char, const Color fg_color, const Color bg_color);

    Frame ppUpscale(const Frame &old_frame, EntitySystem &entitySystem,
                    const int start_x, const int start_y, const unsigned short scale, const unsigned short depth);

    Frame ppBlurLight(const Frame &frame, const unsigned short scale, const float amount);

    Frame compositeLayers(Frame frame, const Frame map_layer, const Frame entity_layer, const Frame ui_layer);

    void render(const Frame &frame);


private:
    bool Initialize();


    unordered_map<wstring, vector<vector<wstring>>> upres_map = {
            {L"?",
                    {
                            {L" ", L"?", L"?", L"?", L" "},
                            {L"?", L"?", L"?", L"?", L"?"},
                            {L"?",  L"?", L"?", L"?", L"?"},
                            {L"?", L"?",  L"?", L"?",  L"?"},
                            {L" ", L"?", L"?", L"?", L" "}}},
            {L" ",
                    {
                            {L" ", L" ", L" ", L" ", L" "},
                            {L" ", L" ", L" ", L" ", L" "},
                            {L" ",  L" ", L" ", L" ", L" "},
                            {L" ", L" ",  L" ", L" ",  L" "},
                            {L" ", L" ", L" ", L" ", L" "}}},
            {L"░",
                    {
                            {L" ", L"░", L" ", L"▓", L" "},
                            {L"░", L"▒", L"░", L"▒", L"░"},
                            {L" ",  L"░", L" ", L"▓", L" "},
                            {L"░", L"▒",  L"░", L"▒",  L"░"},
                            {L" ", L"░", L"▓", L"░", L" "}}},
            {L"▒",
                    {
                            {L"░", L" ", L"▒", L"▓", L"░"},
                            {L" ", L"▒", L"▒", L"▒", L" "},
                            {L"▒",  L"▒", L"▓", L"▒", L"▒"},
                            {L" ", L"▓",  L"▒", L"▓",  L" "},
                            {L"░", L"▒", L"▓", L"▓", L"▒"}}},
            {L"▓",
                    {
                            {L" ", L"▒", L"▓", L"▓", L" "},
                            {L"░", L"▓", L"▓", L"▒", L"░"},
                            {L"▓",  L"▓", L"▓", L"▓", L"▒"},
                            {L"▒", L"▓",  L"▓", L"▓",  L"░"},
                            {L"░", L"▒", L"▓", L"░", L" "}}},
            {L"#",
                    {
                            {L"┍", L"┒", L"┍", L"━", L"┒"},
                            {L"┕", L"┙", L"┕", L"━", L"┙"},
                            {L"┍",  L"━", L"┒", L"┍", L"┒"},
                            {L"┃", L"▒",  L"┃", L"┕",  L"┙"},
                            {L"┕", L"━", L"┙", L"░", L" "}}},
            {L".",
                    {
                            {L" ", L" ", L" ", L" ", L" "},
                            {L" ", L".", L".", L".", L" "},
                            {L" ",  L".", L" ", L".", L" "},
                            {L" ", L".",  L".", L".",  L" "},
                            {L" ", L" ", L" ", L" ", L" "}}},
            {L",",  {
                            {L" ", L" ", L" ", L" ", L" "},
                            {L" ", L" ", L"❁", L" ", L" "},
                            {L" ",  L"⚘", L"🜈", L",", L" "},
                            {L" ", L"\"", L",", L" ",  L" "},
                            {L" ", L" ", L" ", L" ", L" "}}},

            {L"‽",
                    {
                            {L" ", L" ", L" ", L" ", L" "},
                            {L" ", L" ", L" ", L" ", L" "},
                            {L" ",  L"=", L"=", L"=", L" "},
                            {L"+", L"Z",  L"Z", L"Z",  L"+"},
                            {L" ", L"=", L"=", L"=", L" "}}},
    };
};
/*
"""""""5x5 Zombie Brute
" {C  "
"/OO=="
"\===="
"/PQ\ "
"// //"
"""""""
 */