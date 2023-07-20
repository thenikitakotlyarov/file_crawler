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


    SysQuest = QuestSystem();
    if (!SysQuest.running) return false;


    SysLight = LightSystem();
    if (!SysLight.running) return false;

    SysRender = RenderSystem();
    if (!SysRender.running) return false;

    return true;

}

void Game::CleanUp() {
    // Clean up systems


    // Clean up player, monsterSystem, etc.
}


int Game::get_player_level() {
    return round((
                         CURRENT_PLAYER.vitality +
                         CURRENT_PLAYER.power +
                         CURRENT_PLAYER.agility +
                         CURRENT_PLAYER.focus +
                         CURRENT_PLAYER.insight +
                         CURRENT_PLAYER.belief - 40)
                 / (29 + CURRENT_PLAYER.level));
}

void Game::movePlayer(pair<int, int> delta, const int speed) {
    Entity playerEntity = SysEntity.getPlayer();
    for (int i = 0; i < speed; ++i) {
        Intent playerIntent = {playerEntity, IntentType::Move, delta};
        if (CURRENT_PLAYER.current_stamina > 0) {
            if (speed > 1) CURRENT_PLAYER.current_stamina = max(0, CURRENT_PLAYER.current_stamina - 2);
            SysEntity.moveEntity(playerIntent);
        }

    }
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
            CURRENT_MAP = SysMap.genCave(HEIGHT, WIDTH);
            SysEntity.setGameMap(CURRENT_MAP);
            SysEntity.setPlayer(CURRENT_PLAYER);
            SysEntity.spawnPlayer();
            SysEntity.spawnItems(round(0.001333 * (CURRENT_MAP->data.size() * CURRENT_MAP->data[0].size())));
            SysEntity.spawnMonsters(round(0.001333 * (CURRENT_MAP->data.size() * CURRENT_MAP->data[0].size())), 1.0);
            SysLight.setMaps(CURRENT_MAP, SysEntity.getEntities(), WIDTH, HEIGHT);
            SysLight.populateMaps();
            READY_TO_PLAY = true;

        } else {
            if (player_input == 27) paused = !paused;
            if (paused) {
                if (player_input == '0') running = 2;
            } else {// we running bois; take the player's input as commands for the player character

                int player_level = get_player_level();
                if (player_level > CURRENT_PLAYER.level) {// player just leveled up
                    CURRENT_PLAYER.level = player_level;
                    SysEntity.setPlayer(CURRENT_PLAYER);
                    const int item_count = pow(round(0.001333 * (HEIGHT * WIDTH)), (10 + CURRENT_PLAYER.level) / 10);
                    const int monster_count = pow(round(0.00125 * (HEIGHT * WIDTH)), (10 + CURRENT_PLAYER.level) / 10);
                    const float monster_difficulty = pow(1.1, (10 + CURRENT_PLAYER.level) / 10);

                    SysEntity.spawnItems(item_count);
                    SysEntity.spawnMonsters(monster_count, monster_difficulty);//congrats the player with friends!

                }

                if ((player_input == KEY_MOUSE)) { // && getmouse(&event) == OK) {

                    MEVENT event;
                    if (getmouse(&event) == OK) {
                        if (event.bstate & BUTTON1_PRESSED) {
                            Position playerPosition = SysEntity.getPlayerPosition();
                            pair<int, vector<Intent>> strike = CURRENT_PLAYER.primarySkill->Use(
                                    CURRENT_PLAYER, playerPosition,
                                    SysEntity.calculate_fov(playerPosition.x,
                                                            playerPosition.y,
                                                            CURRENT_PLAYER.agility),
                                    SysEntity.getMonsters(), SysEntity.getPositions()
                            );
                            CURRENT_PLAYER.current_energy -= strike.first;
                            for (auto &combat: strike.second) { SysEntity.combatEntities(combat); }
                        } else if (event.bstate & BUTTON3_PRESSED) {
                            Position playerPosition = SysEntity.getPlayerPosition();
                            pair<int, vector<Intent>> strike = CURRENT_PLAYER.secondarySkill->Use(
                                    CURRENT_PLAYER, playerPosition,
                                    SysEntity.calculate_fov(playerPosition.x,
                                                            playerPosition.y,
                                                            CURRENT_PLAYER.agility),
                                    SysEntity.getMonsters(), SysEntity.getPositions()
                            );
                            CURRENT_PLAYER.current_energy -= strike.first;
                            for (auto &combat: strike.second) { SysEntity.combatEntities(combat); }
                        } else if (event.bstate & BUTTON2_PRESSED) {
                            Position playerPosition = SysEntity.getPlayerPosition();
                            pair<int, vector<Intent>> strike = CURRENT_PLAYER.secondarySkill->Use(
                                    CURRENT_PLAYER, playerPosition,
                                    SysEntity.calculate_fov(playerPosition.x,
                                                            playerPosition.y,
                                                            CURRENT_PLAYER.agility),
                                    SysEntity.getMonsters(), SysEntity.getPositions()
                            );
                            CURRENT_PLAYER.current_energy -= strike.first;
                            for (auto &combat: strike.second) { SysEntity.combatEntities(combat); }
                        }
                    }

                } else {
                    //repeat code from above, but only here. doesn't make sense to strip out into a method of Game
                    if (player_input == KEY_F(1)) {
                        Position playerPosition = SysEntity.getPlayerPosition();
                        pair<int, vector<Intent>> strike = CURRENT_PLAYER.primarySkill->Use(
                                CURRENT_PLAYER, playerPosition,
                                SysEntity.calculate_fov(playerPosition.x,
                                                        playerPosition.y,
                                                        CURRENT_PLAYER.agility),
                                SysEntity.getMonsters(), SysEntity.getPositions()
                        );
                        CURRENT_PLAYER.current_energy -= strike.first;
                        for (auto &combat: strike.second) { SysEntity.combatEntities(combat); }
                    } else if (player_input == KEY_F(2)) {
                        Position playerPosition = SysEntity.getPlayerPosition();
                        pair<int, vector<Intent>> strike = CURRENT_PLAYER.secondarySkill->Use(
                                CURRENT_PLAYER, playerPosition,
                                SysEntity.calculate_fov(playerPosition.x,
                                                        playerPosition.y,
                                                        CURRENT_PLAYER.agility),
                                SysEntity.getMonsters(), SysEntity.getPositions()
                        );
                        CURRENT_PLAYER.current_energy -= strike.first;
                        for (auto &combat: strike.second) { SysEntity.combatEntities(combat); }
                    } else if (player_input == KEY_F(3)) {
                        Position playerPosition = SysEntity.getPlayerPosition();
                        pair<int, vector<Intent>> strike = CURRENT_PLAYER.secondarySkill->Use(
                                CURRENT_PLAYER, playerPosition,
                                SysEntity.calculate_fov(playerPosition.x,
                                                        playerPosition.y,
                                                        CURRENT_PLAYER.agility),
                                SysEntity.getMonsters(), SysEntity.getPositions()
                        );
                        CURRENT_PLAYER.current_energy -= strike.first;
                        for (auto &combat: strike.second) { SysEntity.combatEntities(combat); }
                    } else if (player_input == 'w') {
                        movePlayer({0, -1}, 1);
                    } else if (player_input == 'e') {
                        movePlayer({1, -1}, 1);
                    } else if (player_input == 'd') {
                        movePlayer({1, 0}, 1);
                    } else if (player_input == 'c') {
                        movePlayer({1, 1}, 1);
                    } else if (player_input == 'x') {
                        movePlayer({0, 1}, 1);
                    } else if (player_input == 'z') {
                        movePlayer({-1, 1}, 1);
                    } else if (player_input == 'a') {
                        movePlayer({-1, 0}, 1);
                    } else if (player_input == 'q') {
                        movePlayer({-1, -1}, 1);
                    } else if (player_input == 'W') {
                        movePlayer({0, -1}, 1 + CURRENT_PLAYER.agility / 10);
                    } else if (player_input == 'E') {
                        movePlayer({1, -1}, 1 + CURRENT_PLAYER.agility / 10);
                    } else if (player_input == 'D') {
                        movePlayer({1, 0}, 1 + CURRENT_PLAYER.agility / 10);
                    } else if (player_input == 'C') {
                        movePlayer({1, 1}, 1 + CURRENT_PLAYER.agility / 10);
                    } else if (player_input == 'X') {
                        movePlayer({0, 1}, 1 + CURRENT_PLAYER.agility / 10);
                    } else if (player_input == 'Z') {
                        movePlayer({-1, 1}, 1 + CURRENT_PLAYER.agility / 10);
                    } else if (player_input == 'A') {
                        movePlayer({-1, 0}, 1 + CURRENT_PLAYER.agility / 10);
                    } else if (player_input == 'Q') {
                        movePlayer({-1, -1}, 1 + CURRENT_PLAYER.agility / 10);
                    } else if (player_input == ' ') {
                        Position player_pos = SysEntity.getPlayerPosition();
                        const map<Entity, Item> &items = SysEntity.getItems();
                        EntityMap *entityMap = SysEntity.getEntities();
                        for (const auto &entity: entityMap->data[player_pos.x][player_pos.y]) {
                            if (items.find(entity) != items.end()) {
                                items.find(entity)->second.effect(CURRENT_PLAYER);
                                SysEntity.destroyEntity(entity);
                                break;
                            }
                        }
                    } else if (player_input == 0) {
                        if (!get_random_int(0, 99)) {// 1/100 chance of occurring
                            CURRENT_PLAYER.current_health = min(CURRENT_PLAYER.max_health,
                                                                CURRENT_PLAYER.current_health + 1 +
                                                                CURRENT_PLAYER.vitality / 10);
                        }

                        if (!get_random_int(0, 24)) {// 1/25 chance of occurring
                            CURRENT_PLAYER.current_energy = min(CURRENT_PLAYER.max_energy,
                                                                CURRENT_PLAYER.current_energy + 3 +
                                                                CURRENT_PLAYER.focus / 10);

                        }

                        CURRENT_PLAYER.current_stamina = min(CURRENT_PLAYER.max_stamina,
                                                             CURRENT_PLAYER.current_stamina + 1);
                    }
                }
            }


            SysLight.Update(SysEntity.getEntities());

        }

        SysEntity.getCurrentPlayer() = CURRENT_PLAYER;
    }


}


void Game::CARD_TITLE(int y, int x) {
    Frame frame = UISystem::BlankFrame(y, x, 0);

    frame = SysUI.getTitleCard(frame);

    SysRender.render(frame);
}


void Game::MENU_MAIN(int y, int x) {
    Frame frame = UISystem::BlankFrame(y, x, 0);

    frame = SysUI.getMainMenu(frame);

    SysRender.render(frame);
}


void Game::MENU_NEW_GAME(int y, int x) {
    Frame frame = UISystem::BlankFrame(y, x, 0);

    READY_TO_PLAY = false;

    frame = SysUI.getNewGameMenu(frame);

    SysRender.render(frame);
}


void Game::MENU_LOAD_GAME(int y, int x) {
    Frame frame = UISystem::BlankFrame(y, x, 0);

    frame = SysUI.getLoadCard(frame);

    SysRender.render(frame);
}


void Game::MENU_SETTINGS(int y, int x) {
    Frame frame = UISystem::BlankFrame(y, x, 0);

    frame = SysUI.getSettingsMenu(frame);

    SysRender.render(frame);
}


void Game::DEBUG_COLOR(int y, int x) {
    Frame frame = UISystem::BlankFrame(y, x, 0);

    frame = SysUI.getColorDebug(frame);

    SysRender.render(frame);
}


void Game::GAME_OVER(int y, int x) {
    Frame frame = UISystem::BlankFrame(y, x, 0);

    frame = SysUI.getGameOverCard(frame);

    SysRender.render(frame);
}


void Game::PLAY_GAME(int y, int x, const int c_fps) {
    unsigned short resolution = 5;

    CURRENT_PLAYER = CURRENT_PLAYER;
    if (CURRENT_PLAYER.current_health <= 0) {
        running = 999;
    }
    Frame frame = UISystem::BlankFrame(y / resolution, x / resolution, 0);
    Position player_pos = SysEntity.getPlayerPosition();

    int start_x = max(0, player_pos.x - COLS / resolution / 2);
    start_x = min(start_x, WIDTH - COLS / resolution);
    int end_x = start_x + COLS / resolution;
    int start_y = max(0, player_pos.y - (LINES / resolution - 3) / 2);
    start_y = min(start_y, HEIGHT - LINES / resolution - 3);
    int end_y = start_y + LINES / resolution - 3;


    int player_fov_radius = PLAYER_FOV_RADIUS + PLAYER_FOV_RADIUS * CURRENT_PLAYER.focus / 100;

    set<pair<int, int>> veil_fov = SysEntity.calculate_fov(
            player_pos.x, player_pos.y, 2 * player_fov_radius
    );

    set<pair<int, int>> view_fov = SysEntity.calculate_fov(
            player_pos.x, player_pos.y, player_fov_radius
    );

    SysMap.unveilMap(CURRENT_MAP, veil_fov);

    frame = MapSystem::renderMap2D(frame, CURRENT_MAP,
                                   start_y, start_x, end_y, end_x);


    frame = SysLight.renderLighting2D(frame, player_pos, player_fov_radius,
                                      start_y, start_x, end_y, end_x);

    frame = SysEntity.renderEntities2D(frame, view_fov,
                                       start_y, start_x, end_y, end_x);

    frame = MapSystem::renderMap3D(frame, CURRENT_MAP,
                                   start_y, start_x, end_y, end_x);


    SysMap.veilMap(CURRENT_MAP, veil_fov);
    SysMap.forgetMap(CURRENT_MAP, make_pair(player_pos.x, player_pos.y),
                     CURRENT_PLAYER.insight);


    frame = SysRender.ppUpscale(frame, SysEntity, start_x, start_y, resolution);

    frame = SysUI.getTags(frame, SysEntity, start_x, start_y, resolution);
    frame = SysUI.getHud(frame, CURRENT_PLAYER, c_fps);


    frame = SysRender.ppBlurLight(frame, 1, 1.0);

    if (paused) {
        for (int i = 0; i < y; i++) {
            for (int j = 0; j < x; j++) {
                if (i > y / 10 && i < y * 4 / 5 && j > x / 3 && j < x * 2 / 3) {
                    frame.data[i][j].ch = ' ';
                    frame.data[i][j].fg_color = {0, 0, 0};
                    frame.data[i][j].bg_color = {0, 0, 0};
                } else {
                    frame.data[i][j].fg_color = {127, 127, 127};
                    frame.data[i][j].bg_color = {0, 0, 0};
                }
            }
        }
    } else {
        SysEntity.Update();

    }

    SysRender.render(frame);
}


