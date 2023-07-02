#pragma once

#include "systems/interface/UI.h"
#include "systems/generation/GameMap.h"
#include "systems/entity/Entity.h"
#include "components/Frame.h"
#include "components/Intent.h"
#include "templates/player/BaseClasses.h"
#include "helpers.h"

using namespace std;


struct Frame;

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


    Frame CARD_TITLE(int y, int x);

    Frame MENU_MAIN(int y, int x);

    Frame MENU_NEW_GAME(int y, int x);

    Frame MENU_LOAD_GAME(int y, int x);

    Frame MENU_SETTINGS(int y, int x);

    Frame DEBUG_COLOR(int y, int x);

    Frame GAME_OVER(int y, int x);

    Frame PLAY_GAME(int y, int x, const int c_fps);


private:
    bool Initialize();

    MapSystem SysMap;
    EntitySystem SysEntity;

    Player CURRENT_PLAYER;
    GameMap CURRENT_MAP;


};
