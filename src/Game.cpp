#include "Game.h"


Game::Game() {
    bool init_out = Initialize();
    if (!init_out) {
        running = false;
    } else {
        running = true;
    }
    paused = false;

}

Game::~Game() {
    CleanUp();
}

bool Game::Initialize() {
    // Initialize systems
    SysUI = UISystem();
    if (!SysUI.running) return false;

    SysMap = MapSystem();
    if (!SysMap.running) return false;


    SysEntity = EntitySystem();
    if (!SysEntity.running) return false;


    return true;

}

void Game::CleanUp() {
    // Clean up systems


    // Clean up player, monsterSystem, etc.
}

void Game::Update(int player_input) {
    if (running == 1) {// title card
        if (player_input != 0) running = 2;

    } else if (running == 2) {// main menu

        paused = false;//ew wut?
        if (player_input == 27 || player_input == '0') running = 0;
        switch (player_input) {
            case '1':
                running = 3;// send to new game menu
                break;
            case '2':
                running = 4;// send to load game menu
            case '9':
                running = 9;// send to settings menu
            case 'c':
                running = 8;// send to color debug
        }

    } else if (running == 3) {// new game menu
        if (player_input == 27 || player_input == '0') running = 2;
        else if (player_input == '1') {
            CURRENT_PLAYER = getAdventurerTemplate();
            running = 10;
        } else if (player_input == '2') {
            CURRENT_PLAYER = getBarbarianTemplate();
            running = 10;
        } else if (player_input == '3') {
            CURRENT_PLAYER = getBladeTemplate();
            running = 10;
        } else if (player_input == '4') {
            CURRENT_PLAYER = getRogueTemplate();
            running = 10;
        } else if (player_input == '5') {
            CURRENT_PLAYER = getScoutTemplate();
            running = 10;
        } else if (player_input == '6') {
            CURRENT_PLAYER = getRangerTemplate();
            running = 10;
        } else if (player_input == '7') {
            CURRENT_PLAYER = getNecromancerTemplate();
            running = 10;
        } else if (player_input == '8') {
            CURRENT_PLAYER = getSorcererTemplate();
            running = 10;
        } else if (player_input == '9') {
            CURRENT_PLAYER = getMonkTemplate();
            running = 10;
        } else if (tolower(player_input) == 'a') {
            CURRENT_PLAYER = getWizardTemplate();
            running = 10;
        } else if (tolower(player_input) == 'b') {
            CURRENT_PLAYER = getClericTemplate();
            running = 10;
        } else if (tolower(player_input) == 'c') {
            CURRENT_PLAYER = getSpellswordTemplate();
            running = 10;
        } else if (tolower(player_input) == 'd') {
            CURRENT_PLAYER = getPaladinTemplate();
            running = 10;
        } else if (tolower(player_input) == 'e') {
            CURRENT_PLAYER = getRandomTemplate();
            running = 10;
        }

    } else if (running == 4) {// load game menu
        if (player_input == 27 || player_input == '0') running = 2;
        switch (player_input) {
        }
    } else if (running == 8) {// color debug
        if (player_input == 27 || player_input == '0') running = 2;
        switch (player_input) {
        }

    } else if (running == 9) {// settings menu
        if (player_input == 27 || player_input == '0') running = 2;
        switch (player_input) {
        }

    } else if (running <= 10) {// in game
        if (!READY_TO_PLAY) {
            CURRENT_MAP = SysMap.genCave(1024, 1024);
            SysEntity.setGamemap(CURRENT_MAP);
            SysEntity.setPlayer(CURRENT_PLAYER);
            SysEntity.spawnPlayer();
            SysEntity.spawnItems();
            SysEntity.spawnMonsters();
            READY_TO_PLAY = true;

        } else {
            if (player_input == 27) paused = !paused;
            if (paused) {
                if (player_input == '0') running = 2;
            } else {// we running bois; take the player's input as commands for the player character
                if (player_input == 'w') {
                    SysEntity.moveEntity(SysEntity.getPlayer(), make_pair(0, -1));
                } else if (player_input == 'a') {
                    SysEntity.moveEntity(SysEntity.getPlayer(), make_pair(-1, 0));
                } else if (player_input == 's') {
                    SysEntity.moveEntity(SysEntity.getPlayer(), make_pair(0, 1));
                } else if (player_input == 'd') {
                    SysEntity.moveEntity(SysEntity.getPlayer(), make_pair(1, 0));
                }
            }


        }


    }
}


Frame Game::CARD_TITLE(int y, int x) {
    Frame frame = SysUI.BlankFrame(y, x);

    frame = SysUI.getTitleCard(frame);

    return frame;
}


Frame Game::MENU_MAIN(int y, int x) {
    Frame frame = SysUI.BlankFrame(y, x);

    frame = SysUI.getMainMenu(frame);

    return frame;
}


Frame Game::MENU_NEW_GAME(int y, int x) {
    Frame frame = SysUI.BlankFrame(y, x);

    READY_TO_PLAY = false;

    frame = SysUI.getNewGameMenu(frame);

    return frame;
}


Frame Game::MENU_LOAD_GAME(int y, int x) {
    Frame frame = SysUI.BlankFrame(y, x);

    frame = SysUI.getLoadCard(frame);

    return frame;
}


Frame Game::MENU_SETTINGS(int y, int x) {
    Frame frame = SysUI.BlankFrame(y, x);

    frame = SysUI.getSettingsMenu(frame);

    return frame;
}


Frame Game::DEBUG_COLOR(int y, int x) {
    Frame frame = SysUI.BlankFrame(y, x);

    frame = SysUI.getColorDebug(frame);

    return frame;
}


Frame Game::PLAY_GAME(int y, int x) {
    Frame frame = SysUI.BlankFrame(y, x);
    Position player_position = SysEntity.getPlayerPosition();

    int start_x = max(0, player_position.x - COLS / 2);
    start_x = min(start_x, WIDTH - COLS);
    int end_x = start_x + COLS;
    int start_y = max(0, player_position.y - (LINES - 3) / 2);
    start_y = min(start_y, HEIGHT - LINES - 3);
    int end_y = start_y + LINES - 3;


    set<pair<int, int>> current_player_fov = SysEntity.calculate_fov(
            CURRENT_MAP, player_position.x, player_position.y, CURRENT_PLAYER.dexterity
    );

    CURRENT_MAP = SysMap.unveilMap(CURRENT_MAP, current_player_fov);

    //frame = SysRender.r2D(frame, CURRENT_MAP);
    frame = SysMap.renderMap2D(frame, CURRENT_MAP, start_y, start_x, end_y, end_x);
    frame = SysEntity.renderEntities2D(frame, CURRENT_MAP, start_y, start_x, end_y, end_x);


    frame = SysUI.getInGameHud(frame, CURRENT_PLAYER);
    if (paused) {
        for (int i = 0; i < y; i++) {
            for (int j = 0; j < x; j++) {
                if (i > y / 10 && i < y * 4 / 5 && j > x / 3 && j < x * 2 / 3) {
                    frame.data[i][j].first = ' ';
                    frame.data[i][j].second = make_pair(COLOR_BLACK, COLOR_BLACK);
                } else {
                    frame.data[i][j].second = make_pair(COLOR_GREY, COLOR_BLACK);
                }
            }
        }
    }

    SysEntity.Update();

    CURRENT_MAP = SysMap.reveilMap(CURRENT_MAP, current_player_fov);

    //frame = SysRender.bloom(frame);

    return frame;
}


