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


    SysLight = LightSystem();
    if (!SysLight.running) return false;


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
            CURRENT_PLAYER = getDuelistTemplate();
            running = 10;
        } else if (player_input == '4') {
            CURRENT_PLAYER = getFighterTemplate();
            running = 10;
        } else if (player_input == '5') {
            CURRENT_PLAYER = getRogueTemplate();
            running = 10;
        } else if (player_input == '6') {
            CURRENT_PLAYER = getRangerTemplate();
            running = 10;
        } else if (player_input == '7') {
            CURRENT_PLAYER = getDruidTemplate();
            running = 10;
        } else if (player_input == '8') {
            CURRENT_PLAYER = getSorcererTemplate();
            running = 10;
        } else if (player_input == '9') {
            CURRENT_PLAYER = getBardTemplate();
            running = 10;
        } else if (tolower(player_input) == 'a') {
            CURRENT_PLAYER = getWizardTemplate();
            running = 10;
        } else if (tolower(player_input) == 'b') {
            CURRENT_PLAYER = getMonkTemplate();
            running = 10;
        } else if (tolower(player_input) == 'c') {
            CURRENT_PLAYER = getClericTemplate();
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

    } else if (running == 999) {
        if (player_input == 27 || player_input == '0') running = 2;
        switch (player_input) {
        }

    } else if (running <= 10) {// in game
        if (!READY_TO_PLAY) {
            CURRENT_MAP = SysMap.genCave(1024, 1024);
            SysEntity.setGameMap(CURRENT_MAP);
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


                if ((player_input == KEY_MOUSE)) { // && getmouse(&event) == OK) {

                    MEVENT event;
                    if (getmouse(&event) == OK) {
                        if (event.bstate & BUTTON1_PRESSED) {

                            Position playerPosition = SysEntity.getPlayerPosition();

                            pair<int, vector<Intent>> strike = CURRENT_PLAYER.primarySkill->Use(
                                    CURRENT_PLAYER, playerPosition,
                                    SysEntity.calculate_fov(CURRENT_MAP,
                                                            playerPosition.x,
                                                            playerPosition.y,
                                                            CURRENT_PLAYER.agility),
                                    SysEntity.getMonsters(),
                                    SysEntity.getPositions()
                            );
                            SysEntity.getCurrentPlayer().current_energy -= strike.first;
                            for (auto &combat: strike.second) {
                                SysEntity.combatEntities(combat);

                            }


                        } else if (event.bstate & BUTTON3_PRESSED) {

                            Position playerPosition = SysEntity.getPlayerPosition();

                            pair<int, vector<Intent>> strike = CURRENT_PLAYER.secondarySkill->Use(
                                    CURRENT_PLAYER, playerPosition,
                                    SysEntity.calculate_fov(CURRENT_MAP,
                                                            playerPosition.x,
                                                            playerPosition.y,
                                                            CURRENT_PLAYER.agility),
                                    SysEntity.getMonsters(),
                                    SysEntity.getPositions()
                            );
                            SysEntity.getCurrentPlayer().current_energy -= strike.first;
                            for (auto &combat: strike.second) {
                                SysEntity.combatEntities(combat);

                            }


                        }
                    }

                } else if (player_input == 'w') {
                    Entity playerEntity = SysEntity.getPlayer();
                    Intent playerIntent = {playerEntity, IntentType::Move, make_pair(0, -1)};
                    SysEntity.moveEntity(playerIntent);
                } else if (player_input == 'a') {
                    Entity playerEntity = SysEntity.getPlayer();
                    Intent playerIntent = {playerEntity, IntentType::Move, make_pair(-1, 0)};
                    SysEntity.moveEntity(playerIntent);
                } else if (player_input == 's') {
                    Entity playerEntity = SysEntity.getPlayer();
                    Intent playerIntent = {playerEntity, IntentType::Move, make_pair(0, 1)};
                    SysEntity.moveEntity(playerIntent);
                } else if (player_input == 'd') {
                    Entity playerEntity = SysEntity.getPlayer();
                    Intent playerIntent = {playerEntity, IntentType::Move, make_pair(1, 0)};
                    SysEntity.moveEntity(playerIntent);
                } else if (player_input == 'e') {
                    Position player_pos = SysEntity.getPlayerPosition();
                    const map<Entity,Item>& items = SysEntity.getItems();
                    const EntityMap& entityMap = SysEntity.getEntities();
                    for (const auto& entity: entityMap.data[player_pos.x][player_pos.y]) {
                        if (items.find(entity) != items.end()) {
                            items.find(entity)->second.effect(SysEntity.getCurrentPlayer());
                            SysEntity.destroyEntity(entity);
                            break;
                        }
                    }
                } else if (player_input == 0) {
                    if (!get_random_int(0, 99)) {// 1/100 chance of occurring
                        SysEntity.getCurrentPlayer().current_health = min(CURRENT_PLAYER.max_health,
                                                                          CURRENT_PLAYER.current_health + 1 +
                                                                          CURRENT_PLAYER.vitality / 10);
                    }

                    if (!get_random_int(0, 24)) {// 1/25 chance of occurring
                        SysEntity.getCurrentPlayer().current_energy = min(CURRENT_PLAYER.max_energy,
                                                                          CURRENT_PLAYER.current_energy + 1 +
                                                                          CURRENT_PLAYER.focus / 10);

                    }
                }
            }


            SysLight.Update();

        }

        CURRENT_PLAYER = SysEntity.getCurrentPlayer();
    }


}




Frame Game::CARD_TITLE(int y, int x) {
    Frame frame = UISystem::BlankFrame(y, x,0);

    frame = SysUI.getTitleCard(frame);

    return frame;
}


Frame Game::MENU_MAIN(int y, int x) {
    Frame frame = UISystem::BlankFrame(y, x,0);

    frame = SysUI.getMainMenu(frame);

    return frame;
}


Frame Game::MENU_NEW_GAME(int y, int x) {
    Frame frame = UISystem::BlankFrame(y, x,0);

    READY_TO_PLAY = false;

    frame = SysUI.getNewGameMenu(frame);

    return frame;
}


Frame Game::MENU_LOAD_GAME(int y, int x) {
    Frame frame = UISystem::BlankFrame(y, x,0);

    frame = SysUI.getLoadCard(frame);

    return frame;
}


Frame Game::MENU_SETTINGS(int y, int x) {
    Frame frame = UISystem::BlankFrame(y, x,0);

    frame = SysUI.getSettingsMenu(frame);

    return frame;
}


Frame Game::DEBUG_COLOR(int y, int x) {
    Frame frame = UISystem::BlankFrame(y, x,0);

    frame = SysUI.getColorDebug(frame);

    return frame;
}


Frame Game::GAME_OVER(int y, int x) {
    Frame frame = UISystem::BlankFrame(y, x,0);

    frame = SysUI.getGameOverCard(frame);

    return frame;
}


Frame Game::PLAY_GAME(int y, int x, const int c_fps) {
    CURRENT_PLAYER = SysEntity.getCurrentPlayer();
    if (CURRENT_PLAYER.current_health <= 0) {
        running = 999;
    }
    Frame frame = UISystem::BlankFrame(y, x,0);
    Position player_pos = SysEntity.getPlayerPosition();

    int start_x = max(0, player_pos.x - COLS / 2);
    start_x = min(start_x, WIDTH - COLS);
    int end_x = start_x + COLS;
    int start_y = max(0, player_pos.y - (LINES - 3) / 2);
    start_y = min(start_y, HEIGHT - LINES - 3);
    int end_y = start_y + LINES - 3;


    set<pair<int, int>> current_player_fov = EntitySystem::calculate_fov(
            CURRENT_MAP, player_pos.x, player_pos.y, 15 + CURRENT_PLAYER.focus / 10
    );

    CURRENT_MAP = MapSystem::unveilMap(CURRENT_MAP, current_player_fov);

    //frame = SysRender.r2D(frame, CURRENT_MAP);
    frame = MapSystem::renderMap2D(frame, CURRENT_MAP, start_y, start_x, end_y, end_x);
    frame = SysEntity.renderEntities2D(frame, CURRENT_MAP, current_player_fov, start_y, start_x, end_y, end_x);

    frame = SysLight.renderLighting(frame, CURRENT_MAP, player_pos, (PLAYER_FOV_RADIUS+CURRENT_PLAYER.focus/10), start_y, start_x, end_y, end_x);

    frame = SysUI.getInGameHud(frame, SysEntity.getCurrentPlayer(), c_fps);
    if (paused) {
        for (int i = 0; i < y; i++) {
            for (int j = 0; j < x; j++) {
                if (i > y / 10 && i < y * 4 / 5 && j > x / 3 && j < x * 2 / 3) {
                    frame.data[i][j].ch = ' ';
                    frame.data[i][j].fg_color = {0,0,0};
                    frame.data[i][j].bg_color = {0,0,0};
                } else {
                    frame.data[i][j].fg_color = {127,127,127};
                    frame.data[i][j].bg_color = {0,0,0};
                }
            }
        }
    } else {
        SysEntity.Update();

    }


    CURRENT_MAP = MapSystem::veilMap(CURRENT_MAP, current_player_fov);
    CURRENT_MAP = MapSystem::forgetMap(CURRENT_MAP, make_pair(player_pos.x, player_pos.y),
                                       CURRENT_PLAYER.insight);


    //frame = SysRender.bloom(frame);


    return frame;
}


