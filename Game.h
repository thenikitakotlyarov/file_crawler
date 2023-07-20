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

    void CARD_TITLE(int y, int x);

    void MENU_MAIN(int y, int x);

    void MENU_NEW_GAME(int y, int x);

    void MENU_LOAD_GAME(int y, int x);

    void MENU_SETTINGS(int y, int x);

    void DEBUG_COLOR(int y, int x);

    void GAME_OVER(int y, int x);

    void PLAY_GAME(int y, int x, const int c_fps);


private:
    bool Initialize();

    MapSystem SysMap;
    EntitySystem SysEntity;
    QuestSystem SysQuest;
    LightSystem SysLight;
    RenderSystem SysRender;

    Player CURRENT_PLAYER;
    GameMap *CURRENT_MAP;


    void movePlayer(pair<int, int> delta, const int speed);
};
