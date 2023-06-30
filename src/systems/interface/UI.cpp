#include "UI.h"


UISystem::UISystem() {
    bool init_out = Initialize();
    if (!init_out) {
        running = false;
    } else {
        running = true;
    }

}

UISystem::~UISystem() {
    CleanUp();
}

bool UISystem::Initialize() {
    // Initialize systems


    return true;
}

void UISystem::CleanUp() {
    // Clean up systems


    // Clean up player, monsterSystem, etc.
}

void UISystem::Update() {
    // Update entities...

}


Frame &
UISystem::addBar(Frame &frame, int y, int x, int length, float level,
                 wstring icon, pair<int, int> color) {
    for (int i = 0; i <= length; i++) {
        if (x + i >= COLS) break;
        wstring ch;
        pair<int, int> set_color;
        if (i == 1) {
            ch = icon;
        } else if (i == 0 || i == 3) {
            ch = L"⟦";
        } else if (i == 2 || i == length) {
            ch = L"⟧";
        } else {
            ch = L"⟚";
        }


        frame.data[y][x + i].first = ch;

        if (i < 3) {
            frame.data[y][x + i].second.first = color.first;
            frame.data[y][x + i].second.second = COLOR_BLACK;

        } else {
            if (level >= ((float) (i - 3) / (length - 3))) {
                const int bg = color.first;
                set_color = make_pair(COLOR_GREY, bg);
            } else {
                set_color = color;
            }
            frame.data[y][x + i].second = set_color;
        }
    }

    return frame;
}


Frame &
UISystem::addButton(Frame &frame, int y, int x, int height, int width,
                    pair<int, int> color) {
    for (int i = 0; i < height; i++) {
        if (y + i >= LINES) break;
        for (int j = 0; j < width; j++) {
            if (x + j >= COLS) break;
            wstring ch;
            if (i == 0 && j == 0) ch = L"┏";
            else if (i == 0 && j == width - 1) ch = L"┓";
            else if (i == height - 1 && j == 0) ch = L"┗";
            else if (i == height - 1 && j == width - 1) ch = L"┛";
            else if (i == 0 || i == height - 1) ch = L"━";
            else if (j == 0 || j == width - 1) ch = L"┃";
            else ch = L" ";

            frame.data[y + i][x + j].first = ch;
            frame.data[y + i][x + j].second = color;
        }
    }

    return frame;
}


Frame &
UISystem::addText(Frame &frame, int y, int x, wstring text,
                  pair<int, int> color) {
    for (int c = 0; c < text.size(); c++) {
        if (x + c >= COLS) break;
        frame.data[y][x + c].first = text[c];
        frame.data[y][x + c].second = color;
    }

    return frame;
}


Frame
UISystem::BlankFrame(int y, int x) {
    string meta_data = "Blank Frame";
    vector<vector<pair<wstring, pair<int, int>>>> frame_data(
            y,
            vector<pair<wstring, pair<int, int>>>(
                    x, {
                            L"?",
                            make_pair(COLOR_RED, COLOR_BLACK)
                    }
            )
    );


    Frame frame = {
            meta_data,
            frame_data
    };


    return frame;
}


Frame
UISystem::getTitleCard(Frame frame) {
    frame.meta = "Title Card";
    for (int i = 0; i < frame.data.size(); i++) {
        for (int j = 0; j < frame.data[i].size(); j++) {
            frame.data[i][j].first = L"0";


        }
    }

    return frame;
}


Frame
UISystem::getLoadCard(Frame frame) {
    frame.meta = "Title Card";
    for (int i = 0; i < frame.data.size(); i++) {
        for (int j = 0; j < frame.data[i].size(); j++) {
            frame.data[i][j].first = L"L";


        }
    }

    return frame;
}

Frame
UISystem::getMainMenu(Frame frame) {
    frame.meta = "Main Menu";
    for (int i = 0; i < frame.data.size(); i++) {
        for (int j = 0; j < frame.data[i].size(); j++) {
            frame.data[i][j].first = L"1";

        }
    }

    return frame;
}


Frame
UISystem::getNewGameMenu(Frame frame) {
    frame.meta = "New Game Menu";

    frame = addText(frame, 2, 1, L"Please Select a Class:", make_pair(COLOR_WHITE, COLOR_BLACK));

    frame = addText(frame, 5, 1, L"1:  Adventurer!", make_pair(COLOR_WHITE, COLOR_BLACK));

    frame = addText(frame, 6, 1, L"2:", make_pair(COLOR_WHITE, COLOR_BLACK));
    frame = addText(frame, 6, 5, L"Barbarian!", make_pair(COLOR_WHITE, COLOR_BLACK));

    frame = addText(frame, 7, 1, L"3:", make_pair(COLOR_WHITE, COLOR_BLACK));
    frame = addText(frame, 7, 5, L"Blade!", make_pair(COLOR_WHITE, COLOR_BLACK));

    frame = addText(frame, 8, 1, L"4:", make_pair(COLOR_WHITE, COLOR_BLACK));
    frame = addText(frame, 8, 5, L"Rogue!", make_pair(COLOR_WHITE, COLOR_BLACK));

    frame = addText(frame, 9, 1, L"5:", make_pair(COLOR_WHITE, COLOR_BLACK));
    frame = addText(frame, 9, 5, L"Scout!", make_pair(COLOR_WHITE, COLOR_BLACK));

    frame = addText(frame, 10, 1, L"6:", make_pair(COLOR_WHITE, COLOR_BLACK));
    frame = addText(frame, 10, 5, L"Ranger!", make_pair(COLOR_WHITE, COLOR_BLACK));

    frame = addText(frame, 11, 1, L"7:", make_pair(COLOR_WHITE, COLOR_BLACK));
    frame = addText(frame, 11, 5, L"Necromancer!", make_pair(COLOR_WHITE, COLOR_BLACK));

    frame = addText(frame, 12, 1, L"8:", make_pair(COLOR_WHITE, COLOR_BLACK));
    frame = addText(frame, 12, 5, L"Sorcerer!", make_pair(COLOR_WHITE, COLOR_BLACK));

    frame = addText(frame, 13, 1, L"9:", make_pair(COLOR_WHITE, COLOR_BLACK));
    frame = addText(frame, 13, 5, L"Monk!", make_pair(COLOR_WHITE, COLOR_BLACK));

    frame = addText(frame, 14, 1, L"A:", make_pair(COLOR_WHITE, COLOR_BLACK));
    frame = addText(frame, 14, 5, L"Wizard!", make_pair(COLOR_WHITE, COLOR_BLACK));

    frame = addText(frame, 15, 1, L"B:", make_pair(COLOR_WHITE, COLOR_BLACK));
    frame = addText(frame, 15, 5, L"Cleric!", make_pair(COLOR_WHITE, COLOR_BLACK));

    frame = addText(frame, 16, 1, L"C:", make_pair(COLOR_WHITE, COLOR_BLACK));
    frame = addText(frame, 16, 5, L"Spellsword!", make_pair(COLOR_WHITE, COLOR_BLACK));

    frame = addText(frame, 17, 1, L"D:", make_pair(COLOR_WHITE, COLOR_BLACK));
    frame = addText(frame, 17, 5, L"Paladin!", make_pair(COLOR_WHITE, COLOR_BLACK));

    frame = addText(frame, 18, 1, L"E:", make_pair(COLOR_WHITE, COLOR_BLACK));
    frame = addText(frame, 18, 5, L"R", make_pair(COLOR_WHITE, COLOR_BLACK));
    frame = addText(frame, 18, 6, L"A", make_pair(COLOR_WHITE, COLOR_BLACK));
    frame = addText(frame, 18, 7, L"N", make_pair(COLOR_WHITE, COLOR_BLACK));
    frame = addText(frame, 18, 8, L"D", make_pair(COLOR_WHITE, COLOR_BLACK));
    frame = addText(frame, 18, 9, L"O", make_pair(COLOR_WHITE, COLOR_BLACK));
    frame = addText(frame, 18, 10, L"M", make_pair(COLOR_WHITE, COLOR_BLACK));
    frame = addText(frame, 18, 11, L"!", make_pair(COLOR_WHITE, COLOR_BLACK));


    return frame;
}


Frame
UISystem::getLoadGameMenu(Frame frame) {
    frame.meta = "Load Menu";

    frame.data[0][0].first = L"3";


    return frame;
}


Frame
UISystem::getSettingsMenu(Frame frame) {
    frame.meta = "Settings Menu";

    frame.data[0][0].first = L"4";


    return frame;
}


Frame
UISystem::getColorDebug(Frame frame) {
    frame.meta = "Color Debug";

    for (int i = 0; i < frame.data.size(); i++) {
        if (i >= LINES) break;
        for (int j = 0; j < frame.data[i].size(); j++) {
            if (j >= COLS) break;

            if (i == 0) {
                if (j % 2 == 0) {
                    wstring label = int_to_hex_str(j / 2);
                    int fg = (j / 2) % 128;
                    frame = addText(frame, i, j, label, make_pair(fg, 0));
                }
            } else {
                frame.data[i][j].first = L"▒";
                frame.data[i][j].second.first = (j) / 2;
                frame.data[i][j].second.second = i;
            }
        }
    }


    return frame;
}


Frame &UISystem::getPlayerTag(Frame &frame, int y, int x, int player_level, const string &player_class, int color) {
    wchar_t player_level_tag[256];
    wchar_t player_class_tag[256];

    swprintf(player_level_tag, 256, L"LVL%d", player_level);
    wstring ltag = player_level_tag;
    frame = addText(frame, y, x, ltag, make_pair(COLOR_WHITE, COLOR_BLACK));

    frame.data[y][x + ltag.size()].second.first = COLOR_BLACK;
    frame.data[y][x + ltag.size() + 1].second.first = COLOR_BLACK;

    swprintf(player_class_tag, 256, L"%s", player_class.c_str());
    wstring ctag = player_class_tag;
    frame = addText(frame, y, x + ltag.size() + 2, ctag, make_pair(color, COLOR_BLACK));


    frame.data[y][x + ltag.size() + ctag.size() + 2].second.first = COLOR_BLACK;
    frame.data[y][x + ltag.size() + ctag.size() + 3].second.first = COLOR_BLACK;


    return frame;
}

Frame &UISystem::getUiBg(Frame &frame, int dock_height, const pair<int, int> &ui_bg_color) const {
    for (int hy = LINES - dock_height; hy < LINES; ++hy) {
        if (hy < 0) break;
        for (int hx = 0; hx < COLS; ++hx) {
            if (hy == LINES - dock_height) {
                frame.data[hy][hx].first = L"‗";
                frame.data[hy][hx].second = make_pair(COLOR_YELLOW, COLOR_LGREY);
            } else {
                frame.data[hy][hx].first = L"░";
                frame.data[hy][hx].second = ui_bg_color;
            }
        }
    }
    return frame;
}

Frame &
UISystem::getOrb(Frame &frame,
                 int y, int x,
                 int diameter,
                 int level,
                 pair<int, int> color) {
    int orb_size = (diameter * diameter) / 2;
    for (int i = 0; i < (diameter / 2) + 2; ++i) {
        if (y + i >= LINES) break;
        for (int j = 0; j < diameter + 2; ++j) {
            if (x + j >= COLS) break;

            //draw box in center
            if ((j != 0 && i != 0)
                && (j != diameter + 1 && i != 0)
                && (j != 0 && i != diameter / 2 + 1)
                && (j != diameter + 1 && i != diameter / 2 + 1)) {
                pair<int, int> color_pair;
                wstring ch;
                if (level >= orb_size - (((i) * diameter) - j)) {
                    ch = get_random_character({L" ", L" ", L" ", L"~"});
                    color_pair.first = color.first;
                    color_pair.second = color.second;
                } else {
                    ch = get_random_character({L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L"."});
                    color_pair = make_pair(COLOR_GREY, COLOR_BLACK);
                }

                frame.data[y + i][x + j].first = ch.c_str();
                frame.data[y + i][x + j].second = make_pair(color_pair.first, color_pair.second);

            }
            //draw boundary
            if ((i == 0 || i == diameter / 2 + 1) && !(j == 0 || j == diameter + 1)
                || (j == 0 || j == diameter + 1) && !(i == 0 || i == diameter / 2 + 1)
                || (j == 1 || j == diameter) && (i == 1 || i == diameter / 2)) {
                pair<int, int> orb_boundary_color = make_pair(COLOR_BLACK, COLOR_GREY);
                wstring orb_boundary_ch = L"▓";
                if ((j == 1 && i == 1) || (j == diameter && i == diameter / 2)) {
                    orb_boundary_ch = L"▒";
                } else if ((j == 1 && i == diameter / 2) || (j == diameter && i == 1)) {
                    orb_boundary_ch = L"▒";
                } else if ((j == 0 || j == diameter + 1) && (i > 1 && i < diameter / 2)) {
                    orb_boundary_ch = L"⹏";
                } else if ((i == 0 || i == diameter / 2 + 1) && (j > 1 && j < diameter)) {
                    orb_boundary_ch = L"⹏";
                }

                frame.data[y + i][x + j].first = orb_boundary_ch.c_str();
                frame.data[y + i][x + j].second = make_pair(orb_boundary_color.first,
                                                            orb_boundary_color.second);


            }
        }
    }
    return frame;
}


Frame &
UISystem::getAttackSlot(Frame &frame, int y, int x, int height,
                        int skill_index, pair<int, int> color) {
    int width = height * 2;
    for (int i = 0; i < height; i++) {
        if (y + i >= LINES) break;
        for (int j = 0; j < width; j++) {
            if (x + j >= COLS) break;
            wstring ch;
            if (i == 0 && j == 0) ch = L"┏";
            else if (i == 0 && j == width - 1) ch = L"┓";
            else if (i == height - 1 && j == 0) ch = L"┗";
            else if (i == height - 1 && j == width - 1) ch = L"┛";
            else if (i == 0 || i == height - 1) ch = L"━";
            else if (j == 0 || j == width - 1) ch = L"┃";
            else ch = L" ";

            frame.data[y + i][x + j].first = ch;
            frame.data[y + i][x + j].second = color;
        }
    }

    return frame;
}

Frame &
UISystem::getLevelingButton(Frame &frame, int y, int x, int height,
                            bool active) {
    int width = height * 2 - 1;
    pair<int, int> color_pair;


    if (!active) color_pair = make_pair(COLOR_BLACK, COLOR_GREY);
    else color_pair = make_pair(COLOR_BLACK, COLOR_YELLOW);

    frame = addButton(frame, y, x, height, width,
                      color_pair);

    int center_y = height / 2;
    int center_x = width / 2;

    //add a plus sign
    if (!active) {
        frame.data[y + center_y][x + center_x].first = L"☩";
    } else {
        frame.data[y + center_y][x + center_x].first = L"※";
        frame.data[y + center_y][x + center_x].second = make_pair(color_pair.second, color_pair.first);//flip em colors
        frame.data[y + center_y + 1][x + center_x].first = L"⍊";
        frame.data[y + center_y + 1][x + center_x].second = make_pair(color_pair.second,
                                                                      color_pair.first);//flip em colors
        frame.data[y + center_y - 1][x + center_x].first = L"⍑";
        frame.data[y + center_y - 1][x + center_x].second = make_pair(color_pair.second,
                                                                      color_pair.first);//flip em colors
        frame.data[y + center_y][x + center_x + 1].first = L"⎶";
        frame.data[y + center_y][x + center_x + 1].second = make_pair(color_pair.second,
                                                                      color_pair.first);//flip em colors
        frame.data[y + center_y][x + center_x + 2].first = L"⟧";
        frame.data[y + center_y][x + center_x + 2].second = make_pair(color_pair.second,
                                                                      color_pair.first);//flip em colors
        frame.data[y + center_y][x + center_x - 1].first = L"⎶";
        frame.data[y + center_y][x + center_x - 1].second = make_pair(color_pair.second,
                                                                      color_pair.first);//flip em colors
        frame.data[y + center_y][x + center_x - 2].first = L"⟦";
        frame.data[y + center_y][x + center_x - 2].second = make_pair(color_pair.second,
                                                                      color_pair.first);//flip em colors
    }


    return frame;
}

Frame &
UISystem::getPotionBar(Frame &frame, int y, int x, int slot_height, int slot_width, pair<int, int> color) {
    frame = addButton(frame, y, x, slot_height, slot_width, make_pair(COLOR_GREY, COLOR_BLACK));
    frame = addText(frame, y, x, L"1", color);
    frame = addButton(frame, y, x + slot_width, slot_height, slot_width, make_pair(COLOR_GREY, COLOR_BLACK));
    frame = addText(frame, y, x + slot_width, L"2", color);
    frame = addButton(frame, y, x + slot_width * 2, slot_height, slot_width, make_pair(COLOR_GREY, COLOR_BLACK));
    frame = addText(frame, y, x + slot_width * 2, L"3", color);
    frame = addButton(frame, y, x + slot_width * 3, slot_height, slot_width, make_pair(COLOR_GREY, COLOR_BLACK));
    frame = addText(frame, y, x + slot_width * 3, L"4", color);
    return frame;
}


Frame
UISystem::getInGameHud(Frame frame, Player player) {
    //draw level and class name
    int y, x;


    //draw background for hud

    pair<int, int> ui_bg_color = make_pair(COLOR_LGREY, COLOR_BLACK);
    int dock_height = 6;

    frame = getUiBg(frame, dock_height, ui_bg_color);


    int orb_diameter = (dock_height + 1) * 2;
    //it's actually half this value on the y, takes 1/6th of the screen, smollest orb size of 3x7


    //draw health orb
    y = max(0, min(LINES - 1, LINES - (orb_diameter / 2) - 2));
    x = max(0, min(COLS - 1, 1));

    int health_level = 100;// static_cast<int>(static_cast<float>(playerStats.health) / PLAYER_MAX_HP * orb_size);
    frame = getOrb(frame, y, x, orb_diameter,
                   health_level, make_pair(COLOR_BLACK, COLOR_RED));


    //draw left attack slot
    y = max(0, min(LINES - 1, LINES - dock_height + 1));
    x = max(0, min(COLS - 1, orb_diameter + 4));

    frame = getAttackSlot(frame, y, x, dock_height - 1,
                          0, make_pair(COLOR_GREY, COLOR_BLACK));

    //draw left attribute button
    y = max(0, min(LINES - 1, y + 1));
    x = max(0, min(COLS - 1, x + (dock_height - 1) * 2 + 1));

    frame = getLevelingButton(frame, y, x, dock_height - 3,
                              false);

    //draw player info
    x = max(0, min(COLS - 1, x + (dock_height - 3) * 2 + 1));
    frame = getPlayerTag(frame, y, x, player.level, player.class_name, player.color);

    //draw experience bar
    y = max(0, min(LINES - 1, y + 1));
    frame = addBar(frame, y, x,
                   COLS / 6, 0.1f, L"☆",
                   make_pair(COLOR_WHITE, COLOR_BLACK));

    //draw stamina bar
    y = max(0, min(LINES - 1, y + 1));
    frame = addBar(frame, y, x,
                   COLS / 6, 1.0f, L"ᗢ",
                   make_pair(COLOR_ORANGE, COLOR_BLACK));



    //draw character menu buttons?





    //draw energy orb
    y = max(0, min(LINES - 1, LINES - (orb_diameter / 2) - 2));
    x = max(0, min(COLS - 1, COLS - orb_diameter - 3));
    int energy_level = 100;// static_cast<int>(static_cast<float>(playerStats.energy) / PLAYER_MAX_EP * orb_size);

    frame = getOrb(frame, y, x,
                   orb_diameter, energy_level,
                   make_pair(COLOR_WHITE, player.color));



    //draw right attack slot
    y = max(0, min(LINES - 1, LINES - dock_height + 1));
    x = max(0, min(COLS - 1, x - (dock_height - 1) * 2 - 1));

    frame = getAttackSlot(frame, y, x,
                          dock_height - 1, 0,
                          make_pair(COLOR_GREY, COLOR_BLACK));


    //draw right skills button
    y = max(0, min(LINES - 1, y + 1));
    x = max(0, min(COLS - 1, x - (dock_height - 3) * 2 - 1));

    frame = getLevelingButton(frame, y, x,
                              dock_height - 3,
                              true);

    //draw potion slots
    x = max(0, min(COLS - 1, x - (dock_height - 3) * 2 * 4 - 3));
    frame = getPotionBar(frame, y, x,
                         dock_height - 3, (dock_height - 3) * 2,
                         make_pair(COLOR_WHITE, COLOR_BLACK));

    ////draw combat log
    //int log_size = min(LINES - orb_diameter - 2, static_cast<int>(combat_log.size()));
    //
    //
    //for (int i = 0; i < log_size; i++) {
    //    mvaddstr(y + i, x, combat_log[combat_log.size() - log_size + i].c_str());
    //}


    return frame;
}
