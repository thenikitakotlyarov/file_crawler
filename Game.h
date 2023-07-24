#pragma once


#include "helpers.h"
#include "constants.h"
//
//#include "systems/entity/Entity.h"
//#include "systems/interface/UI.h"
//#include "systems/generation/GameMap.h"
//#include "systems/graphics/Lighting.h"

#include "include.h"

using namespace std;


class Game {
public:
    explicit Game();

    ~Game();

    int running;
    bool paused;
    bool READY_TO_PLAY = false;


    UISystem SysUI;


    void CleanUp();

    void Update(int player_input);

    int get_player_level();

    void CARD_TITLE();

    void MENU_MAIN();

    void MENU_NEW_GAME();

    void MENU_LOAD_GAME();

    void MENU_SETTINGS();

    void DEBUG_COLOR();

    void GAME_OVER();

    void PLAY_GAME(const int c_fps);


private:
    bool Initialize();

    MapSystem SysMap;
    EntitySystem SysEntity;
    QuestSystem SysQuest;
    LightSystem SysLight;
    RenderSystem SysRender;

    Player CURRENT_PLAYER;
    GameMap *CURRENT_MAP;


    bool settings_graphics_upscale;
    bool settings_graphics_3D;
    bool settings_graphics_lighting;
    bool settings_graphics_bloom;
    bool settings_ui_tags;
    bool settings_ui_hud;

    const vector<pair<wstring, bool &>> pause_menu = {
            {L"Upscale",settings_graphics_upscale},
            {L"Pseudo3D",settings_graphics_3D},
            {L"Lighting",settings_graphics_lighting},
            {L"Bloom",settings_graphics_bloom},
            {L"Tags",settings_ui_tags},
            {L"UI",settings_ui_hud},
    };

    void movePlayer(pair<int, int> delta, const int speed);
};
