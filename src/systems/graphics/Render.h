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

    Sprite charToSprite(EntitySystem &entitySystem, const Position pos, const wstring this_char,
                        const Color fg_color, const Color bg_color);

    Frame ppUpscale(const Frame &frame, EntitySystem &entitySystem,
                    const int start_x, const int start_y, const unsigned short scale);

    Frame ppBlurLight(const Frame &frame, const unsigned short scale, const float amount);

    void render(const Frame &frame);


private:
    bool Initialize();

    unordered_map<wstring, vector<vector<wstring>>> upres_map = {
//            {L"@",
//                    {
//                            {L" ", L" ", L"@", L" ", L" "},
//                            {L" ", L"/", L"Ꙭ", L"\\", L" "},
//                            {L"|", L" ", L"B", L" ", L"|"},
//                            {L" ", L"|", L"x", L"|", L" "},
//                            {L" ", L"|", L" ", L"|", L" "}}},

            {L"@",
                    {
                            {L" ", L" ", L"@", L" ", L" "},
                            {L" ", L"!", L"W", L"!", L" "},
                            {L" ",  L"|", L"Y", L"|", L" "},
                            {L" ", L" ",  L"Λ", L" ",  L" "},
                            {L" ", L"J", L" ", L"L", L" "}}},
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
            {L"o",
                    {
                            {L" ", L" ", L"⛁", L" ", L" "},
                            {L" ", L"╭", L"▒", L"╮", L" "},
                            {L" ",  L"│", L"▓", L"│", L" "},
                            {L" ", L"╰",  L"─", L"╯",  L" "},
                            {L" ", L" ", L" ", L" ", L" "}}},
            {L"⍟",
                    {
                            {L" ", L"╔", L"╤", L"╗", L" "},
                            {L"╔", L" ", L" ", L" ", L"╗"},
                            {L"╞",  L" ", L"🏶", L" ", L"╡"},
                            {L"╚", L" ",  L" ", L" ",  L"╝"},
                            {L" ", L"╚", L"╨", L"╝", L" "}}},
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
            {L"M",
                    {
                            {L" ", L"{", L"C", L" ", L" "},
                            {L"/", L"O", L"o", L"=", L"="},
                            {L"\\", L"=", L"=", L"=", L" "},
                            {L"/", L"P",  L"Q", L"\\", L" "},
                            {L"/", L"/", L" ", L"/", L"/"}}},
            {L"m",
                    {
                            {L" ", L"{", L"C", L" ", L" "},
                            {L"/", L"O", L"o", L"=", L"="},
                            {L"\\", L"=", L"=", L"=", L" "},
                            {L"/", L"P",  L"Q", L"\\", L" "},
                            {L"/", L"/", L" ", L"/", L"/"}}},
            {L"n",
                    {
                            {L" ", L"{", L"C", L" ", L" "},
                            {L"/", L"O", L"o", L"=", L"="},
                            {L"\\", L"=", L"=", L"=", L" "},
                            {L"/", L"P",  L"Q", L"\\", L" "},
                            {L"/", L"/", L" ", L"/", L"/"}}},
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