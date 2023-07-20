#pragma once

using namespace std;

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "components/graphics/Color.h"
//#include "components/graphics/RawColor.h"

#define FPS 60
#define DEV_LOG_FILE "dev_log.txt"
#define GAME_LOG_FILE = "game_log.txt"
#define UI_LOG_COUNT 7
#define WIDTH  1000
#define HEIGHT  1000
#define PLAYER_MAX_HP 100
#define PLAYER_MAX_EP 150
#define PLAYER_FOV_RADIUS 12
#define PLAYER_MEMORY_RADIUS (3*PLAYER_FOV_RADIUS)
#define sound false
#define PLAYER_FOOTSTEP_SOUND_COUNT 6
#define PLAYER_TILE L"ᚤ"

const Color NCOLOR_BLACK = {
        0,
        0,
        0
};
const Color NCOLOR_DGREY = {
        63,
        63,
        63
};

const Color NCOLOR_MGREY = {
        127,
        127,
        127
};
const Color NCOLOR_LGREY = {
        191,
        191,
        191
};
const Color NCOLOR_WHITE = {
        255,
        255,
        255
};

const Color NCOLOR_RED = {
        170,
        0,
        0
};

const Color NCOLOR_DRED = {
        63,
        0,
        0
};
const Color NCOLOR_MRED = {
        127,
        0,
        0
};
const Color NCOLOR_LRED = {
        191,
        0,
        0
};

const Color NCOLOR_ORANGE = {
        229,
        126,
        23
};

const Color NCOLOR_YELLOW = {
        255,
        255,
        85
};
const Color NCOLOR_DYELLOW = {
        63,
        63,
        0
};
const Color NCOLOR_MYELLOW = {
        127,
        127,
        0
};
const Color NCOLOR_LYELLOW = {
        191,
        191,
        0
};

const Color NCOLOR_GREEN = {
        0,
        255,
        0
};
const Color NCOLOR_DGREEN = {
        0,
        63,
        0
};
const Color NCOLOR_MGREEN = {
        0,
        127,
        0
};
const Color NCOLOR_LGREEN = {
        0,
        191,
        0
};

const Color NCOLOR_CYAN = {
        0,
        255,
        255
};
const Color NCOLOR_DCYAN = {
        0,
        63,
        63
};
const Color NCOLOR_MCYAN = {
        0,
        127,
        127
};
const Color NCOLOR_LCYAN = {
        0,
        191,
        191
};

const Color NCOLOR_AMARINE = {
        0,
        128,
        255
};
const Color NCOLOR_BLUE = {
        0,
        0,
        255
};
const Color NCOLOR_DBLUE = {
        0,
        0,
        63
};
const Color NCOLOR_MBLUE = {
        0,
        0,
        127
};
const Color NCOLOR_LBLUE = {
        0,
        0,
        191
};

const Color NCOLOR_PURPLE = {
        126,
        23,
        229
};
const Color NCOLOR_PINK = {
        255,
        85,
        255
};
const Color NCOLOR_MAGENTA = {
        255,
        0,
        255
};
const Color NCOLOR_DMAGENTA = {
        63,
        0,
        63
};
const Color NCOLOR_MMAGENTA = {
        127,
        0,
        127
};
const Color NCOLOR_LMAGENTA = {
        191,
        0,
        191
};

const Color NCOLOR_BROWN = {
        93,
        58,
        29
};

//
//const vector<wstring> GROUND_TILES = {
//        L".", L".", L".", L".", L".", L".", L".", L".", L".", L".", L".", L".", L".",
//        L".", L".", L".",
//        L"·", L"·", L"·", L"·", L"·", L"·", L"·", L"·", L"·", L"·", L"·", L"·", L"·",
//        L"·", L"·", L"·",
//        L",", L",", L",", L",", L",", L",", L",", L",", L",", L",", L",", L",", L",",
//        L",", L",", L",",
//        L";", L";", L";", L";", L";", L";", L";", L";", L";", L";", L";", L";", L";",
//        L";", L";", L";",
//        L":", L":", L":", L":", L":", L":", L":", L":", L":", L":", L":", L":", L":",
//        L":", L":", L":",
//        L"⁏", L"⁏", L"⁏", L"⁏", L"⁏", L"⁏", L"⁏", L"⁏", L"⁏", L"⁏", L"⁏", L"⁏", L"⁏",
//        L"⁏", L"⁏", L"⁏",
//        L"ʻ", L"ʻ", L"ʻ", L"ʻ", L"ʻ", L"ʻ", L"ʻ", L"ʻ", L"ʻ", L"ʻ", L"ʻ", L"ʻ", L"ʻ",
//        L"ʻ", L"ʻ", L"ʻ",
//        L"ʼ", L"ʼ", L"ʼ", L"ʼ", L"ʼ", L"ʼ", L"ʼ", L"ʼ", L"ʼ", L"ʼ", L"ʼ", L"ʼ", L"ʼ",
//        L"ʼ", L"ʼ", L"ʼ",
//        L"'", L"'", L"'", L"'", L"'", L"'", L"'", L"'", L"'", L"'", L"'", L"'", L"'",
//        L"'", L"'", L"'",
//        L"‘", L"‘", L"‘", L"‘", L"‘", L"‘", L"‘", L"‘", L"‘", L"‘", L"‘", L"‘", L"‘",
//        L"‘", L"‘", L"‘",
//        L"’", L"’", L"’", L"’", L"’", L"’", L"’", L"’", L"’", L"’", L"’", L"’", L"’",
//        L"’", L"’", L"’",
//        L"“", L"“", L"“", L"“", L"“", L"“", L"“", L"“", L"“", L"“", L"“", L"“", L"“",
//        L"“", L"“", L"“",
//        L"”", L"”", L"”", L"”", L"”", L"”", L"”", L"”", L"”", L"”", L"”", L"”", L"”",
//        L"”", L"”", L"”",
//        L"¤", L"✿", L"❀", L"❁", L"✾", L"•", L"⁎", L"*", L"°"};//plaaaaaants
//const vector<wstring> TRAP_TILES = {
//        L"¶", L"§", L"‡", L"†", L"‽", L"∮", L"∯", L"∱", L"∲", L"∳"};
//const vector<wstring> MONSTER_LOW_HP_TILES = {
//        L"а", L"б", L"в", L"г", L"д", L"е", L"ж", L"з", L"и", L"к", L"л", L"м",
//        L"н", L"о", L"п", L"р", L"с", L"т", L"у", L"ф", L"х", L"ц", L"ч", L"ш",
//        L"щ", L"ъ", L"ы", L"ь", L"э", L"ю", L"я", L"і", L"ї", L"є"};
//const vector<wstring> MONSTER_MID_HP_TILES = {
//        L"α", L"β", L"γ", L"δ", L"ε", L"ζ", L"η", L"θ", L"ι", L"κ", L"λ", L"μ",
//        L"ν", L"ξ", L"ο", L"π", L"ρ", L"ς", L"σ", L"τ", L"υ", L"φ", L"χ", L"ψ",
//        L"ω", L"ά", L"έ", L"ή", L"ί", L"ό", L"ύ", L"ώ", L"ϑ", L"ϒ", L"ϓ", L"ϔ"};
//const vector<wstring> MONSTER_TILES = {
//        L"آ", L"أ", L"إ", L"ء", L"ؤ", L"ئ", L"ب", L"ت", L"ث", L"ج", L"ح", L"خ",
//        L"د", L"ذ", L"ر", L"ز", L"س", L"ش", L"ص", L"ض", L"ط", L"ظ", L"ع", L"غ",
//        L"ـ", L"ف", L"ق", L"ك", L"ل", L"م", L"ن"};


const unordered_set<wstring> VOID_TILES = {L" "};
const unordered_set<wstring> GROUND_TILES = {L","};
const unordered_set<wstring> WALL_TILES = {L"▓"};
const unordered_set<wstring> TRAP_TILES = {L"‽"};
const unordered_set<wstring> MONSTER_TILES = {L"M"};
const unordered_set<wstring> ITEM_TILES = {L"o",L"⍟"};
