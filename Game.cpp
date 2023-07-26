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


    settings_graphics_upscale = false;
    settings_graphics_3D = true;
    settings_graphics_lighting = true;
    settings_graphics_bloom = false;
    settings_ui_tags = false;
    settings_ui_hud = true;


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
        if (SysEntity.getCurrentPlayer().stamina.first > 0) {
            if (speed > 1)
                SysEntity.getCurrentPlayer().stamina.first = max(0,
                                                                 SysEntity.getCurrentPlayer().stamina.first -
                                                                 2);
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
                if (player_input == '0') {
                    running = 2;
                } else if (tolower(player_input) == 'w' || player_input == KEY_UP) {
                    SysUI.menu_position = (6 + SysUI.menu_position - 1) % 6;
                } else if (tolower(player_input) == 'x' || player_input == KEY_DOWN) {
                    SysUI.menu_position = (SysUI.menu_position + 1) % 6;
                } else if (tolower(player_input) == 'd' || player_input == KEY_RIGHT
                           || tolower(player_input) == 'a' || player_input == KEY_LEFT) {
                    pause_menu[SysUI.menu_position].second = !pause_menu[SysUI.menu_position].second;
                }

            } else {// we running bois; take the player's input as commands for the player character

                int player_level = get_player_level();
                if (player_level > SysEntity.getCurrentPlayer().level) {// player just leveled up
                    SysEntity.getCurrentPlayer().level = player_level;

                    SysEntity.getCurrentPlayer().health.second = SysEntity.getCurrentPlayer().health.first;
                    SysEntity.getCurrentPlayer().energy.first = SysEntity.getCurrentPlayer().energy.second;
                    SysEntity.getCurrentPlayer().stamina.first = SysEntity.getCurrentPlayer().stamina.second;

                    //SysEntity.setPlayer(SysEntity.getCurrentPlayer());
                    const int item_count = pow(round(0.001333 * (HEIGHT * WIDTH)),
                                               (10 + SysEntity.getCurrentPlayer().level) / 10);
                    const int monster_count = pow(round(0.00125 * (HEIGHT * WIDTH)),
                                                  (10 + SysEntity.getCurrentPlayer().level) / 10);
                    const float monster_difficulty = pow(1.1, (10 + SysEntity.getCurrentPlayer().level) / 10);

                    SysEntity.spawnItems(item_count);
                    SysEntity.spawnMonsters(monster_count, monster_difficulty);//congrats the player with friends!

                }

                if ((player_input == KEY_MOUSE)) { // && getmouse(&event) == OK) {

                    MEVENT event;
                    if (getmouse(&event) == OK) {
                        if (event.bstate & BUTTON1_PRESSED) {
                            Position playerPosition = SysEntity.getPlayerPosition();
                            pair<int, vector<Intent>> strike = SysEntity.getCurrentPlayer().primarySkill->Use(
                                    SysEntity.getCurrentPlayer(), playerPosition,
                                    SysEntity.calculate_fov(playerPosition.x,
                                                            playerPosition.y,
                                                            SysEntity.getCurrentPlayer().agility),
                                    SysEntity.getMonsters(), SysEntity.getPositions()
                            );
                            SysEntity.getCurrentPlayer().energy.first -= strike.first;
                            for (auto &combat: strike.second) { SysEntity.combatEntities(combat); }
                        } else if (event.bstate & BUTTON3_PRESSED) {
                            Position playerPosition = SysEntity.getPlayerPosition();
                            pair<int, vector<Intent>> strike = SysEntity.getCurrentPlayer().secondarySkill->Use(
                                    SysEntity.getCurrentPlayer(), playerPosition,
                                    SysEntity.calculate_fov(playerPosition.x,
                                                            playerPosition.y,
                                                            SysEntity.getCurrentPlayer().agility),
                                    SysEntity.getMonsters(), SysEntity.getPositions()
                            );
                            SysEntity.getCurrentPlayer().energy.first -= strike.first;
                            for (auto &combat: strike.second) { SysEntity.combatEntities(combat); }
                        } else if (event.bstate & BUTTON2_PRESSED) {
                            Position playerPosition = SysEntity.getPlayerPosition();
                            pair<int, vector<Intent>> strike = SysEntity.getCurrentPlayer().secondarySkill->Use(
                                    SysEntity.getCurrentPlayer(), playerPosition,
                                    SysEntity.calculate_fov(playerPosition.x,
                                                            playerPosition.y,
                                                            SysEntity.getCurrentPlayer().agility),
                                    SysEntity.getMonsters(), SysEntity.getPositions()
                            );
                            SysEntity.getCurrentPlayer().energy.first -= strike.first;
                            for (auto &combat: strike.second) { SysEntity.combatEntities(combat); }
                        }
                    }

                } else {
                    //repeat code from above, but only here. doesn't make sense to strip out into a method of Game
                    if (player_input == '1') {
                        //TODO: implement potion use on slot 1
                        if (SysEntity.getCurrentPlayer().potion_belt.content[0].quantity) {
                            const int I_AM_A_DEBUG = break_point();
                            SysEntity.getCurrentPlayer().potion_belt.content[0].item->effect(
                                    SysEntity.getCurrentPlayer());
                            SysEntity.getCurrentPlayer().potion_belt.content[0].quantity -= 1;
                            if (SysEntity.getCurrentPlayer().potion_belt.content[0].quantity <= 0) {
                                SysEntity.getCurrentPlayer().potion_belt.content[0].item = new Item(getNullItem());
                                SysEntity.getCurrentPlayer().potion_belt.content[0].quantity = 0;
                            }
                        }

                    } else if (player_input == '2') {
                        //TODO: implement potion use on slot 2
                        if (SysEntity.getCurrentPlayer().potion_belt.content[1].quantity) {
                            const int I_AM_A_DEBUG = break_point();
                            SysEntity.getCurrentPlayer().potion_belt.content[1].item->effect(
                                    SysEntity.getCurrentPlayer());
                            SysEntity.getCurrentPlayer().potion_belt.content[1].quantity -= 1;
                            if (SysEntity.getCurrentPlayer().potion_belt.content[1].quantity <= 0) {
                                SysEntity.getCurrentPlayer().potion_belt.content[1].item = new Item(getNullItem());
                                SysEntity.getCurrentPlayer().potion_belt.content[1].quantity = 0;
                            }
                        }
                    } else if (player_input == '3') {
                        //TODO: implement potion use on slot 3
                        if (SysEntity.getCurrentPlayer().potion_belt.content[2].quantity) {
                            const int I_AM_A_DEBUG = break_point();
                            SysEntity.getCurrentPlayer().potion_belt.content[2].item->effect(
                                    SysEntity.getCurrentPlayer());
                            SysEntity.getCurrentPlayer().potion_belt.content[2].quantity -= 1;
                            if (SysEntity.getCurrentPlayer().potion_belt.content[2].quantity <= 0) {
                                SysEntity.getCurrentPlayer().potion_belt.content[2].item = new Item(getNullItem());
                                SysEntity.getCurrentPlayer().potion_belt.content[2].quantity = 0;
                            }
                        }
                    } else if (player_input == '4') {
                        Position playerPosition = SysEntity.getPlayerPosition();
                        pair<int, vector<Intent>> strike = SysEntity.getCurrentPlayer().primarySkill->Use(
                                SysEntity.getCurrentPlayer(), playerPosition,
                                SysEntity.calculate_fov(playerPosition.x,
                                                        playerPosition.y,
                                                        SysEntity.getCurrentPlayer().agility),
                                SysEntity.getMonsters(), SysEntity.getPositions()
                        );
                        SysEntity.getCurrentPlayer().energy.first -= strike.first;
                        for (auto &combat: strike.second) { SysEntity.combatEntities(combat); }
                    } else if (player_input == '5') {
                        Position playerPosition = SysEntity.getPlayerPosition();
                        pair<int, vector<Intent>> strike = SysEntity.getCurrentPlayer().secondarySkill->Use(
                                SysEntity.getCurrentPlayer(), playerPosition,
                                SysEntity.calculate_fov(playerPosition.x,
                                                        playerPosition.y,
                                                        SysEntity.getCurrentPlayer().agility),
                                SysEntity.getMonsters(), SysEntity.getPositions()
                        );
                        SysEntity.getCurrentPlayer().energy.first -= strike.first;
                        for (auto &combat: strike.second) { SysEntity.combatEntities(combat); }
                    } else if (player_input == '6') {
                        Position playerPosition = SysEntity.getPlayerPosition();
                        pair<int, vector<Intent>> strike = SysEntity.getCurrentPlayer().secondarySkill->Use(
                                SysEntity.getCurrentPlayer(), playerPosition,
                                SysEntity.calculate_fov(playerPosition.x,
                                                        playerPosition.y,
                                                        SysEntity.getCurrentPlayer().agility),
                                SysEntity.getMonsters(), SysEntity.getPositions()
                        );
                        SysEntity.getCurrentPlayer().energy.first -= strike.first;
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
                        movePlayer({0, -1}, 1 + SysEntity.getCurrentPlayer().agility / 10);
                    } else if (player_input == 'E') {
                        movePlayer({1, -1}, 1 + SysEntity.getCurrentPlayer().agility / 10);
                    } else if (player_input == 'D') {
                        movePlayer({1, 0}, 1 + SysEntity.getCurrentPlayer().agility / 10);
                    } else if (player_input == 'C') {
                        movePlayer({1, 1}, 1 + SysEntity.getCurrentPlayer().agility / 10);
                    } else if (player_input == 'X') {
                        movePlayer({0, 1}, 1 + SysEntity.getCurrentPlayer().agility / 10);
                    } else if (player_input == 'Z') {
                        movePlayer({-1, 1}, 1 + SysEntity.getCurrentPlayer().agility / 10);
                    } else if (player_input == 'A') {
                        movePlayer({-1, 0}, 1 + SysEntity.getCurrentPlayer().agility / 10);
                    } else if (player_input == 'Q') {
                        movePlayer({-1, -1}, 1 + SysEntity.getCurrentPlayer().agility / 10);
                    } else if (player_input == ' ' || player_input == '\n') {
                        Position player_pos = SysEntity.getPlayerPosition();
                        const map<Entity, Item> &items = SysEntity.getItems();
                        for (const auto &entity: SysEntity.getEntities()->data[player_pos.x][player_pos.y]) {
                            if (items.find(entity) != items.end()) {
                                const auto &this_item = items.find(entity);
                                ItemType this_type = this_item->second.type;
                                if (is_in_set(this_type, PotionType)) {

                                    for (auto &slot: SysEntity.getCurrentPlayer().potion_belt.content) {
                                        if (slot.item->type == this_type
                                            && slot.quantity + 1 <= slot.size) {
                                            slot.quantity++;
                                            SysEntity.destroyEntity(entity);
                                            break;
                                        } else if (slot.quantity == 0) {
                                            slot.item = new Item{this_item->second};
                                            slot.quantity = 1;
                                            SysEntity.destroyEntity(entity);
                                            break;

                                        }
                                    }
                                } else if (is_in_set(this_type, OrbType)) {
                                    this_item->second.effect(SysEntity.getCurrentPlayer());
                                    SysEntity.destroyEntity(entity);
                                }

                            }
                        }
                    } else if (player_input == 0) {
                        if (!get_random_int(0, 99)) {// 1/100 chance of occurring
                            SysEntity.getCurrentPlayer().health.second = min(SysEntity.getCurrentPlayer().health.second,
                                                                             SysEntity.getCurrentPlayer().health.first +
                                                                             1 +
                                                                             SysEntity.getCurrentPlayer().vitality /
                                                                             10);
                        }

                        if (!get_random_int(0, 24)) {// 1/25 chance of occurring
                            SysEntity.getCurrentPlayer().energy.first = min(SysEntity.getCurrentPlayer().energy.second,
                                                                            SysEntity.getCurrentPlayer().energy.first +
                                                                            1 +
                                                                            SysEntity.getCurrentPlayer().focus / 10);

                        }

                        SysEntity.getCurrentPlayer().stamina.first = min(SysEntity.getCurrentPlayer().stamina.second,
                                                                         SysEntity.getCurrentPlayer().stamina.first +
                                                                         1);
                    }
                }
            }


            SysLight.Update(SysEntity.getEntities());

        }

        CURRENT_PLAYER = SysEntity.getCurrentPlayer();
    }


}


void Game::CARD_TITLE() {
    const int y = LINES, x = COLS;
    Frame frame = UISystem::BlankFrame(y, x, 0);

    frame = SysUI.getTitleCard(frame);

    SysRender.render(frame);
}


void Game::MENU_MAIN() {
    const int y = LINES, x = COLS;
    Frame frame = UISystem::BlankFrame(y, x, 0);

    frame = SysUI.getMainMenu(frame);

    SysRender.render(frame);
}


void Game::MENU_NEW_GAME() {
    const int y = LINES, x = COLS;
    Frame frame = UISystem::BlankFrame(y, x, 0);

    READY_TO_PLAY = false;

    frame = SysUI.getNewGameMenu(frame);

    SysRender.render(frame);
}


void Game::MENU_LOAD_GAME() {
    const int y = LINES, x = COLS;
    Frame frame = UISystem::BlankFrame(y, x, 0);

    frame = SysUI.getLoadCard(frame);

    SysRender.render(frame);
}


void Game::MENU_SETTINGS() {
    const int y = LINES, x = COLS;
    Frame frame = UISystem::BlankFrame(y, x, 0);

    frame = SysUI.getSettingsMenu(frame);

    SysRender.render(frame);
}


void Game::DEBUG_COLOR() {
    const int y = LINES, x = COLS;
    Frame frame = UISystem::BlankFrame(y, x, 0);

    frame = SysUI.getColorDebug(frame);

    SysRender.render(frame);
}


void Game::GAME_OVER() {
    const int y = LINES, x = COLS;
    Frame frame = UISystem::BlankFrame(y, x, 0);

    frame = SysUI.getGameOverCard(frame);

    SysRender.render(frame);
}


void Game::PLAY_GAME(const int c_fps) {
    const int y = LINES, x = COLS;
    unsigned short resolution = 1;
    if (settings_graphics_upscale) resolution = 5;

    SysEntity.setPlayer(CURRENT_PLAYER);
    if (CURRENT_PLAYER.health.second <= 0) {
        running = 999;
    }
    Frame frame = UISystem::BlankFrame(y, x, 0);
    Frame layer_map = UISystem::BlankFrame(y / resolution, x / resolution, 0);
    Frame layer_entity = UISystem::BlankFrame(y / resolution, x / resolution, 0);
    Frame layer_ui = UISystem::BlankFrame(y, x, 0);

    const Position player_pos = SysEntity.getPlayerPosition();

    const int start_x = max(0, min(player_pos.x - COLS / resolution / 2, WIDTH - COLS / resolution)),
            end_x = start_x + COLS / resolution;
    const int start_y = max(0, min(player_pos.y - LINES / resolution / 2, HEIGHT - LINES / resolution)),
            end_y = start_y + LINES / resolution;


    const int player_fov_radius = PLAYER_FOV_RADIUS + PLAYER_FOV_RADIUS * CURRENT_PLAYER.focus / 100;

    const set<pair<int, int>> veil_fov = SysEntity.calculate_fov(
            player_pos.x, player_pos.y, 2 * player_fov_radius
    );

    const set<pair<int, int>> view_fov = SysEntity.calculate_fov(
            player_pos.x, player_pos.y, player_fov_radius
    );

    SysMap.unveilMap(CURRENT_MAP, veil_fov);

    layer_map = MapSystem::renderMap2D(layer_map, CURRENT_MAP,
                                       start_y, start_x, end_y, end_x);

    if (settings_graphics_lighting) {
        layer_map = SysLight.renderLighting2D(layer_map, player_pos, player_fov_radius,
                                              start_y, start_x, end_y, end_x);
    }

    if (settings_graphics_3D) {
        layer_map = MapSystem::renderMap3D(layer_map, CURRENT_MAP,
                                           start_y, start_x, end_y, end_x);
    }

    layer_entity = SysEntity.renderEntities2D(layer_entity, layer_map, view_fov,
                                              start_y, start_x, end_y, end_x);


    SysMap.veilMap(CURRENT_MAP, veil_fov);
    SysMap.forgetMap(CURRENT_MAP, make_pair(player_pos.x, player_pos.y),
                     CURRENT_PLAYER.insight);

    if (settings_graphics_upscale) {
        layer_map = SysRender.ppUpscale(layer_map, SysEntity, start_x, start_y, resolution, 2);
        layer_entity = SysRender.ppUpscale(layer_entity, SysEntity, start_x, start_y, resolution, 1);
    }

    if (settings_graphics_bloom) {
        layer_map = SysRender.ppBlurLight(layer_map, 2, 1.2);
    }


    if (settings_ui_tags) {
        layer_ui = SysUI.getTags(layer_ui, SysEntity, view_fov, start_x, start_y, resolution);
    }

    if (settings_ui_hud) {
        layer_ui = SysUI.getHud(layer_ui, CURRENT_PLAYER, c_fps);
    }


    frame = SysRender.compositeLayers(frame, layer_map, layer_entity, layer_ui);

    if (paused) {
        frame = SysUI.overlayPause(frame, pause_menu);
    } else {
        SysEntity.Update();

    }

    SysRender.render(frame);
}


