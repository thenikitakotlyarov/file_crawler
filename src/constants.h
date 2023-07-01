#pragma once

using namespace std;
#include <vector>
#include <string>
#define COLOR_BLACK     0
#define COLOR_GREY      1
#define COLOR_LGREY     2
#define COLOR_WHITE     3
#define COLOR_RED       4
#define COLOR_ORANGE    5
#define COLOR_YELLOW    6
#define COLOR_LGREEN    7
#define COLOR_GREEN     8
#define COLOR_CYAN      9
#define COLOR_AMARINE   10
#define COLOR_LBLUE     11
#define COLOR_BLUE      12
#define COLOR_PURPLE    13
#define COLOR_PINK      14
#define COLOR_MAGENTA   15
#define FPS 30
#define DEV_LOG_FILE "dev_log.txt"
#define GAME_LOG_FILE = "game_log.txt"
#define UI_LOG_COUNT 7
#define WIDTH  1000
#define HEIGHT  1000
#define PLAYER_MAX_HP 100
#define PLAYER_MAX_EP 150
#define PLAYER_FOV_RADIUS 12
#define PLAYER_MEMORY_RADIUS 3*PLAYER_FOV_RADIUS
#define sound false
#define PLAYER_FOOTSTEP_SOUND_COUNT 6
#define PLAYER_TILE L"ᚤ"

const vector<wstring> GROUND_TILES = {L"⠁",L"⠂",L"⠃",L"⠄",L"⠅",L"⠆",L"⠇",L"⠈",L"⠉",L"⠊",L"⠋",L"⠌",L"⠍",L"⠎",L"⠏",L"⠐",L"⠑",L"⠒",L"⠓",L"⠔",L"⠕",L"⠖",L"⠗",L"⠘",L"⠙",L"⠚",L"⠛",L"⠜",L"⠝",L"⠞",L"⠟",L"⠠",L"⠡",L"⠢",L"⠣",L"⠤",L"⠥",L"⠦",L"⠧",L"⠨",L"⠩",L"⠪",L"⠫",L"⠬",L"⠭",L"⠮",L"⠯",L"⠰",L"⠱",L"⠲",L"⠳",L"⠴",L"⠵",L"⠶",L"⠷",L"⠸",L"⠹",L"⠺",L"⠻",L"⠼",L"⠽",L"⠾",L"⠿",L"⡀",L"⡁",L"⡂",L"⡃",L"⡄",L"⡅",L"⡆",L"⡇",L"⡈",L"⡉",L"⡊",L"⡋",L"⡌",L"⡍",L"⡎",L"⡏",L"⡐",L"⡑",L"⡒",L"⡓",L"⡔",L"⡕",L"⡖",L"⡗",L"⡘",L"⡙",L"⡚",L"⡛",L"⡜",L"⡝",L"⡞",L"⡟",L"⡠",L"⡡",L"⡢",L"⡣",L"⡤",L"⡥",L"⡦",L"⡧",L"⡨",L"⡩",L"⡪",L"⡫",L"⡬",L"⡭",L"⡮",L"⡯",L"⡰",L"⡱",L"⡲",L"⡳",L"⡴",L"⡵",L"⡶",L"⡷",L"⡸",L"⡹",L"⡺",L"⡻",L"⡼",L"⡽",L"⡾",L"⡿",L"⢀",L"⢁",L"⢂",L"⢃",L"⢄",L"⢅",L"⢆",L"⢇",L"⢈",L"⢉",L"⢊",L"⢋",L"⢌",L"⢍",L"⢎",L"⢏",L"⢐",L"⢑",L"⢒",L"⢓",L"⢔",L"⢕",L"⢖",L"⢗",L"⢘",L"⢙",L"⢚",L"⢛",L"⢜",L"⢝",L"⢞",L"⢟",L"⢠",L"⢡",L"⢢",L"⢣",L"⢤",L"⢥",L"⢦",L"⢧",L"⢨",L"⢩",L"⢪",L"⢫",L"⢬",L"⢭",L"⢮",L"⢯",L"⢰",L"⢱",L"⢲",L"⢳",L"⢴",L"⢵",L"⢶",L"⢷",L"⢸",L"⢹",L"⢺",L"⢻",L"⢼",L"⢽",L"⢾",L"⢿",L"⣀",L"⣁",L"⣂",L"⣃",L"⣄",L"⣅",L"⣆",L"⣇",L"⣈",L"⣉",L"⣊",L"⣋",L"⣌",L"⣍",L"⣎",L"⣏",L"⣐",L"⣑",L"⣒",L"⣓",L"⣔",L"⣕",L"⣖",L"⣗",L"⣘",L"⣙",L"⣚",L"⣛",L"⣜",L"⣝",L"⣞",L"⣟",L"⣠",L"⣡",L"⣢",L"⣣",L"⣤",L"⣥",L"⣦",L"⣧",L"⣨",L"⣩",L"⣪",L"⣫",L"⣬",L"⣭",L"⣮",L"⣯",L"⣰",L"⣱",L"⣲",L"⣳",L"⣴",L"⣵",L"⣶",L"⣷",L"⣸",L"⣹",L"⣺",L"⣻",L"⣼",L"⣽",L"⣾",L"⣿"};
const vector<wstring> WALL_TILES = {L"▓"};
const vector<wstring> TRAP_TILES = {L"¤", L"¶", L"§", L"‡", L"†", L"‽", L"∮", L"∯", L"∱", L"∲", L"∳"};
const vector<wstring> VOID_TILES = {L" "};
const vector<wstring> MONSTER_LOW_HP_TILES = {L"а", L"б", L"в", L"г", L"д", L"е", L"ж", L"з", L"и", L"к", L"л", L"м", L"н", L"о", L"п", L"р", L"с", L"т", L"у", L"ф", L"х", L"ц", L"ч", L"ш", L"щ", L"ъ", L"ы", L"ь", L"э", L"ю", L"я", L"і", L"ї", L"є"};
const vector<wstring> MONSTER_MID_HP_TILES = {L"α", L"β", L"γ", L"δ", L"ε", L"ζ", L"η", L"θ", L"ι", L"κ", L"λ", L"μ", L"ν", L"ξ", L"ο", L"π", L"ρ", L"ς", L"σ", L"τ", L"υ", L"φ", L"χ", L"ψ", L"ω", L"ά", L"έ", L"ή", L"ί", L"ό", L"ύ", L"ώ", L"ϑ", L"ϒ", L"ϓ", L"ϔ"};
const vector<wstring> MONSTER_HIGH_HP_TILES = {L"آ", L"أ", L"إ", L"ء", L"ؤ", L"ئ", L"ب", L"ت", L"ث", L"ج", L"ح", L"خ", L"د", L"ذ", L"ر", L"ز", L"س", L"ش", L"ص", L"ض", L"ط", L"ظ", L"ع", L"غ", L"ـ", L"ف", L"ق", L"ك", L"ل", L"م", L"ن"};