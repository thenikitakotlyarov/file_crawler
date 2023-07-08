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


}

void UISystem::Update() {

}


Frame &
UISystem::addBar(Frame &frame, int y, int x, int length, float level,
                 const wstring &icon, Color fg_color, Color bg_color) {
    for (int i = 0; i <= length; i++) {
        if (x + i >= COLS) break;
        wstring ch;
        pair<Color, Color> set_color;
        if (i == 1) {
            ch = icon;
        } else if (i == 0 || i == 3) {
            ch = L"⟦";
        } else if (i == 2 || i == length) {
            ch = L"⟧";
        } else {
            ch = L"⟚";
        }


        frame.data[y][x + i].ch = ch;

        if (i < 3) {
            frame.data[y][x + i].fg_color = fg_color;
            frame.data[y][x + i].bg_color = NCOLOR_BLACK;

        } else {
            if (level >= ((float) (i - 3) / (length - 3))) {
                const Color bg = fg_color;
                set_color = make_pair(NCOLOR_GREY, bg);
            } else {
                set_color = make_pair(fg_color, bg_color);
            }
            frame.data[y][x + i].fg_color = set_color.first;
            frame.data[y][x + i].bg_color = set_color.second;
        }
    }

    return frame;
}


Frame &
UISystem::addButton(Frame &frame, int y, int x, int height, int width,
                    Color fg_color, Color bg_color) {
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

            frame.data[y + i][x + j].ch = ch;
            frame.data[y + i][x + j].fg_color = fg_color;
            frame.data[y + i][x + j].bg_color = bg_color;
        }
    }

    return frame;
}


Frame &
UISystem::addText(Frame &frame, int y, int x, wstring text,
                  Color fg_color, Color bg_color) {
    for (int c = 0; c < text.size(); c++) {
        if (x + c >= COLS) break;
        frame.data[y][x + c].ch = text[c];
        frame.data[y][x + c].fg_color = fg_color;
        frame.data[y][x + c].bg_color = bg_color;
    }

    return frame;
}


Frame
UISystem::BlankFrame(int y, int x, unsigned long frame_count) {
    FrameMeta meta_data = {
            frame_count,
            "Blank Frame",
            ""
    };


    vector<vector<Pixel>> frame_data(
            y,
            vector<Pixel>(
                    x, {
                            L" ",
                            NCOLOR_RED,
                            NCOLOR_BLACK
                    }
            )
    );


    Frame frame = {
            meta_data,
            frame_data
    };


    return frame;
}

Frame UISystem::getFpsLabel(Frame &frame, const int c_fps) {
    wstringstream ss;
    ss << L"FPS: " << c_fps;

    wstring fps_text = ss.str();
    int fps_text_length = fps_text.size();

    // Top right position
    int x_position = COLS - fps_text_length;
    int y_position = 0;  // Top row

    Color fg_color = NCOLOR_WHITE;
    Color bg_color = NCOLOR_BLACK;

    frame = addText(frame, y_position, x_position, fps_text, fg_color, bg_color);

    return frame;
}


Frame
UISystem::getTitleCard(Frame frame) {
    frame.meta.name = "Title Card";
    for (int i = 0; i < frame.data.size(); i++) {
        for (int j = 0; j < frame.data[i].size(); j++) {
            frame.data[i][j].ch = L"0";


        }
    }

    return frame;
}


Frame
UISystem::getLoadCard(Frame frame) {
    frame.meta.name = "Title Card";
    for (int i = 0; i < frame.data.size(); i++) {
        for (int j = 0; j < frame.data[i].size(); j++) {
            frame.data[i][j].ch = L"L";


        }
    }

    return frame;
}


Frame UISystem::getGameOverCard(Frame frame) {
    frame.meta.name = "Game Over Card";
    for (int i = 0; i < frame.data.size(); i++) {
        for (int j = 0; j < frame.data[i].size(); j++) {
            if ((j + 1) % 3 != 0) {
                frame.data[i][j].ch = L"G";
            } else {
                frame.data[i][j].ch = L"z";
            }
        }
    }


    return frame;
}


Frame
UISystem::getMainMenu(Frame frame) {
    frame.meta.name = "Main Menu";
    for (int i = 0; i < frame.data.size(); i++) {
        for (int j = 0; j < frame.data[i].size(); j++) {
            frame.data[i][j].ch = L"1";

        }
    }

    return frame;
}


Frame
UISystem::getNewGameMenu(Frame frame) {
    frame.meta.name = "New Game Menu";

    frame = addText(frame, 2, 1, L"Please Select a Class:", NCOLOR_WHITE, NCOLOR_BLACK);

    frame = addText(frame, 5, 1, L"1:  Adventurer!", NCOLOR_WHITE, NCOLOR_BLACK);

    frame = addText(frame, 6, 1, L"2:", NCOLOR_WHITE, NCOLOR_BLACK);
    frame = addText(frame, 6, 5, L"Barbarian!", NCOLOR_RED, NCOLOR_BLACK);

    frame = addText(frame, 7, 1, L"3:", NCOLOR_WHITE, NCOLOR_BLACK);
    frame = addText(frame, 7, 5, L"Duelist!", NCOLOR_ORANGE, NCOLOR_BLACK);

    frame = addText(frame, 8, 1, L"4:", NCOLOR_WHITE, NCOLOR_BLACK);
    frame = addText(frame, 8, 5, L"Fighter!", NCOLOR_YELLOW, NCOLOR_BLACK);

    frame = addText(frame, 9, 1, L"5:", NCOLOR_WHITE, NCOLOR_BLACK);
    frame = addText(frame, 9, 5, L"Rogue!", NCOLOR_LGREEN, NCOLOR_BLACK);

    frame = addText(frame, 10, 1, L"6:", NCOLOR_WHITE, NCOLOR_BLACK);
    frame = addText(frame, 10, 5, L"Ranger!", NCOLOR_GREEN, NCOLOR_BLACK);

    frame = addText(frame, 11, 1, L"7:", NCOLOR_WHITE, NCOLOR_BLACK);
    frame = addText(frame, 11, 5, L"Druid!", NCOLOR_CYAN, NCOLOR_BLACK);

    frame = addText(frame, 12, 1, L"8:", NCOLOR_WHITE, NCOLOR_BLACK);
    frame = addText(frame, 12, 5, L"Sorcerer!", NCOLOR_AMARINE, NCOLOR_BLACK);

    frame = addText(frame, 13, 1, L"9:", NCOLOR_WHITE, NCOLOR_BLACK);
    frame = addText(frame, 13, 5, L"Bard!", NCOLOR_LBLUE, NCOLOR_BLACK);

    frame = addText(frame, 14, 1, L"A:", NCOLOR_WHITE, NCOLOR_BLACK);
    frame = addText(frame, 14, 5, L"Wizard!", NCOLOR_BLUE, NCOLOR_BLACK);

    frame = addText(frame, 15, 1, L"B:", NCOLOR_WHITE, NCOLOR_BLACK);
    frame = addText(frame, 15, 5, L"Monk!", NCOLOR_PURPLE, NCOLOR_BLACK);

    frame = addText(frame, 16, 1, L"C:", NCOLOR_WHITE, NCOLOR_BLACK);
    frame = addText(frame, 16, 5, L"Cleric!", NCOLOR_PINK, NCOLOR_BLACK);

    frame = addText(frame, 17, 1, L"D:", NCOLOR_WHITE, NCOLOR_BLACK);
    frame = addText(frame, 17, 5, L"Paladin!", NCOLOR_MAGENTA, NCOLOR_BLACK);

    frame = addText(frame, 18, 1, L"E:", NCOLOR_WHITE, NCOLOR_BLACK);
    frame = addText(frame, 18, 5, L"R", NCOLOR_WHITE, NCOLOR_BLACK);
    frame = addText(frame, 18, 6, L"A", NCOLOR_WHITE, NCOLOR_BLACK);
    frame = addText(frame, 18, 7, L"N", NCOLOR_WHITE, NCOLOR_BLACK);
    frame = addText(frame, 18, 8, L"D", NCOLOR_WHITE, NCOLOR_BLACK);
    frame = addText(frame, 18, 9, L"O", NCOLOR_WHITE, NCOLOR_BLACK);
    frame = addText(frame, 18, 10, L"M", NCOLOR_WHITE, NCOLOR_BLACK);
    frame = addText(frame, 18, 11, L"!", NCOLOR_WHITE, NCOLOR_BLACK);


    return frame;
}


Frame
UISystem::getLoadGameMenu(Frame frame) {
    frame.meta.name = "Load Menu";

    frame.data[0][0].ch = L"3";


    return frame;
}


Frame
UISystem::getSettingsMenu(Frame frame) {
    frame.meta.name = "Settings Menu";

    frame.data[0][0].ch = L"4";


    return frame;
}


Frame
UISystem::getColorDebug(Frame frame) {
    frame.meta.name = "Color Debug";
//
//    for (int i = 0; i < frame.data.size(); i++) {
//        if (i >= LINES) break;
//        for (int j = 0; j < frame.data[i].size(); j++) {
//            if (j >= COLS) break;
//
//            if (i == 0) {
//                if (j % 2 == 0) {
//                    wstring label = int_to_hex_str(j / 2);
//                    int fg = (j / 2) % 128;
//                    frame = addText(frame, i, j, label, make_pair(fg, 0));
//                }
//            } else {
//                frame.data[i][j].ch = L"▒";
//                frame.data[i][j].second.first = (j) / 2;
//                frame.data[i][j].second.second = i;
//            }
//        }
//    }


    return frame;
}


Frame &UISystem::getPlayerTag(Frame &frame, int y, int x, int player_level, const string &player_class, Color fg_color,
                              Color bg_color) {
    wchar_t player_level_tag[256];
    wchar_t player_class_tag[256];

    swprintf(player_level_tag, 256, L"LVL%d", player_level);
    wstring ltag = player_level_tag;
    frame = addText(frame, y, x, ltag, NCOLOR_WHITE, NCOLOR_BLACK);

    frame.data[y][x + ltag.size()].fg_color = NCOLOR_BLACK;
    frame.data[y][x + ltag.size() + 1].bg_color = NCOLOR_BLACK;

    swprintf(player_class_tag, 256, L"%s", player_class.c_str());
    wstring ctag = player_class_tag;
    frame = addText(frame, y, x + ltag.size() + 2, ctag, fg_color, bg_color);


    frame.data[y][x + ltag.size() + ctag.size() + 2].fg_color = bg_color;
    frame.data[y][x + ltag.size() + ctag.size() + 3].bg_color = bg_color;


    return frame;
}

Frame &UISystem::getUiBg(Frame &frame, int dock_height, Color fg_color, Color bg_color) {
    for (int hy = LINES - dock_height; hy < LINES; ++hy) {
        if (hy < 0) break;
        for (int hx = 0; hx < COLS; ++hx) {
            if (hy == LINES - dock_height) {
                frame.data[hy][hx].ch = L"‗";
                frame.data[hy][hx].fg_color = NCOLOR_YELLOW;
                frame.data[hy][hx].bg_color = NCOLOR_LGREY;
            } else {
                frame.data[hy][hx].ch = L"░";
                frame.data[hy][hx].fg_color = fg_color;
                frame.data[hy][hx].bg_color = bg_color;
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
                 Color fg_color, Color bg_color) {
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
                pair<Color, Color> color_pair;
                wstring ch;
                if (level >= 100 * (orb_size - (((i) * diameter) - j)) / orb_size) {
                    ch = get_random_character({L" ", L" ", L" ", L"~"});
                    color_pair.first = fg_color;
                    color_pair.second = bg_color;
                } else {
                    ch = get_random_character({L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L"."});
                    color_pair = make_pair(NCOLOR_GREY, NCOLOR_BLACK);
                }

                frame.data[y + i][x + j].ch = ch;
                frame.data[y + i][x + j].fg_color = color_pair.first;
                frame.data[y + i][x + j].bg_color = color_pair.second;

            }
            //draw boundary
            if ((i == 0 || i == diameter / 2 + 1) && !(j == 0 || j == diameter + 1)
                || (j == 0 || j == diameter + 1) && !(i == 0 || i == diameter / 2 + 1)
                || (j == 1 || j == diameter) && (i == 1 || i == diameter / 2)) {
                pair<Color, Color> orb_boundary_color = make_pair(NCOLOR_BLACK, NCOLOR_GREY);
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

                frame.data[y + i][x + j].ch = orb_boundary_ch;
                frame.data[y + i][x + j].fg_color = orb_boundary_color.first;
                frame.data[y + i][x + j].bg_color = orb_boundary_color.second;
            }
        }
    }
    return frame;
}


Frame &
UISystem::getAttackSlot(Frame &frame, int y, int x, int height,
                        vector<vector<wstring>> icon, Color fg_color, Color bg_color) {
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
            else if (i > 0 && j > 0
                     && i <= icon.size()
                     && j <= icon[0].size())
                ch = icon[i - 1][j - 1];
            else ch = L" ";

            frame.data[y + i][x + j].ch = ch;
            frame.data[y + i][x + j].fg_color = fg_color;
            frame.data[y + i][x + j].bg_color = bg_color;
        }
    }

    return frame;
}

Frame &
UISystem::getMenuButton(Frame &frame, int y, int x, int height,
                        vector<vector<wstring>> icon, bool active) {
    int width = height * 2 - 1;
    pair<Color, Color> color_pair;


    if (!active) color_pair = make_pair(NCOLOR_BLACK, NCOLOR_GREY);
    else color_pair = make_pair(NCOLOR_BLACK, NCOLOR_YELLOW);

    frame = addButton(frame, y, x, height, width,
                      color_pair.first, color_pair.second);

    int center_y = height / 2;
    int center_x = width / 2;

    //add a plus sign
    if (!active) {
        for (int i = 0; i < icon.size(); i++) {
            for (int j = 0; j < icon[0].size(); j++) {
                if (icon[i][j] != L" ") {
                    frame.data[y + i][x + j].ch = icon[i][j];

                }
            }
        }
    } else {
        frame.data[y + center_y][x + center_x].ch = L"※";
        frame.data[y + center_y][x + center_x].fg_color = color_pair.second;
        frame.data[y + center_y][x + center_x].bg_color = color_pair.first;//flip em colors

        frame.data[y + center_y + 1][x + center_x].ch = L"⍊";
        frame.data[y + center_y + 1][x + center_x].fg_color = color_pair.second;
        frame.data[y + center_y + 1][x + center_x].bg_color = color_pair.first;//flip em colors

        frame.data[y + center_y - 1][x + center_x].ch = L"⍑";
        frame.data[y + center_y - 1][x + center_x].fg_color = color_pair.second;
        frame.data[y + center_y + 1][x + center_x].bg_color = color_pair.first;//flip em colors

        frame.data[y + center_y][x + center_x + 1].ch = L"⎶";
        frame.data[y + center_y][x + center_x + 1].fg_color = color_pair.second;
        frame.data[y + center_y][x + center_x + 1].bg_color = color_pair.first;//flip em colors

        frame.data[y + center_y][x + center_x + 2].ch = L"⟧";
        frame.data[y + center_y][x + center_x + 2].fg_color = color_pair.second;
        frame.data[y + center_y][x + center_x + 2].bg_color = color_pair.first;//flip em colors

        frame.data[y + center_y][x + center_x - 1].ch = L"⎶";
        frame.data[y + center_y][x + center_x - 1].fg_color = color_pair.second;
        frame.data[y + center_y][x + center_x - 1].bg_color = color_pair.first;//flip em colors

        frame.data[y + center_y][x + center_x - 2].ch = L"⟦";
        frame.data[y + center_y][x + center_x - 2].fg_color = color_pair.second;
        frame.data[y + center_y][x + center_x - 2].bg_color = color_pair.first;//flip em colors

    }


    return frame;
}

Frame &
UISystem::getPotionBar(Frame &frame, int y, int x, int slot_height, int slot_width, Color fg_color, Color bg_color) {
    frame = addButton(frame, y, x, slot_height, slot_width, NCOLOR_GREY, NCOLOR_BLACK);
    frame = addText(frame, y, x, L"1", fg_color, bg_color);
    frame = addButton(frame, y, x + slot_width, slot_height, slot_width, NCOLOR_GREY, NCOLOR_BLACK);
    frame = addText(frame, y, x + slot_width, L"2", fg_color, bg_color);
    frame = addButton(frame, y, x + slot_width * 2, slot_height, slot_width, NCOLOR_GREY, NCOLOR_BLACK);
    frame = addText(frame, y, x + slot_width * 2, L"3", fg_color, bg_color);
    frame = addButton(frame, y, x + slot_width * 3, slot_height, slot_width, NCOLOR_GREY, NCOLOR_BLACK);
    frame = addText(frame, y, x + slot_width * 3, L"4", fg_color, bg_color);
    return frame;
}


Frame
UISystem::getInGameHud(Frame frame, const Player &player, const int c_fps) {
    //draw level and class name
    int y, x;


    //draw background for hud

    pair<Color, Color> ui_bg_color = make_pair(NCOLOR_LGREY, NCOLOR_BLACK);
    int dock_height = 6;

    frame = getUiBg(frame, dock_height, ui_bg_color.first, ui_bg_color.second);


    int orb_diameter = (dock_height + 1) * 2;
    //it's actually half this value on the y, takes 1/6th of the screen, smollest orb size of 3x7


    //draw health orb
    y = max(0, min(LINES - 1, LINES - (orb_diameter / 2) - 2));
    x = max(0, min(COLS - 1, 1));

    int health_level = static_cast<int>(static_cast<float>(player.current_health) / player.max_health * 100);
    frame = getOrb(frame, y, x, orb_diameter,
                   health_level, NCOLOR_BLACK, NCOLOR_RED);


    //draw left attack slot
    y = max(0, min(LINES - 1, LINES - dock_height + 1));
    x = max(0, min(COLS - 1, orb_diameter + 4));

    frame = getAttackSlot(frame, y, x, dock_height - 1,
                          player.primarySkill->icon, NCOLOR_GREY, NCOLOR_BLACK);


    //draw player info
    y = max(0, min(LINES - 1, y + 1));
    x = max(0, min(COLS - 1, x + (dock_height - 1) * 2 + 1));
    frame = getPlayerTag(frame, y, x, player.level, player.class_name, player.color, NCOLOR_BLACK);

    //draw experience bar
    y = max(0, min(LINES - 1, y + 1));
    frame = addBar(frame, y, x,
                   (dock_height - 3) * 2 * 4 - 4, 0.1f, L"☆",
                   NCOLOR_WHITE, NCOLOR_BLACK);

    //draw stamina bar
    y = max(0, min(LINES - 1, y + 1));
    int stamina_level = static_cast<int>(static_cast<float>(player.current_stamina) / player.max_stamina);
    frame = addBar(frame, y, x,
                   (dock_height - 3) * 2 * 4 - 4, stamina_level, L"ᗢ",
                   NCOLOR_ORANGE, NCOLOR_BLACK);



    //draw character menu buttons?


    //draw left attributes button
    y = max(0, min(LINES - 1, LINES - dock_height + 2));
    x = max(0, min(COLS - 1, (COLS / 2) - (dock_height - 3) * 2 * 3 / 2 - 1));

    frame = getMenuButton(frame, y, x, dock_height - 3,
                          {
                                  {L"A", L" ", L" "},
                                  {L" ", L" ", L"♗"}
                                  //TODO:come up with coolor icons
                                  //{L"A", L" ", L"▁", L" ", L" "},
                                  //{L" ", L"▟", L"⍶", L"▙", L" "},
                                  //{L" ", L"▟", L"║", L"▙", L" "}
                          },
                          false);


    //draw center inventory button
    x = max(0, min(COLS - 1, x + (dock_height - 3) * 2));

    frame = getMenuButton(frame, y, x, dock_height - 3,
                          {
                                  {L"I", L" ", L" "},
                                  {L" ", L" ", L"⌹"}
                                  //{L"I", L"▄", L"▁", L"▄", L" "},
                                  //{L" ", L"☋", L"█", L"☋", L" "},
                                  //{L" ", L"▚", L"█", L"▞", L" "}
                          },
                          false);

    //draw right skills button
    x = max(0, min(COLS - 1, x + (dock_height - 3) * 2));

    frame = getMenuButton(frame, y, x,
                          dock_height - 3,
                          {
                                  {L"S", L" ", L" "},
                                  {L" ", L" ", L"⁂"}
                                  //{L"S", L" ", L"█", L" ", L" "},
                                  //{L" ", L"▚", L"☆", L"▞", L" "},
                                  //{L" ", L"▟", L"▀", L"▙", L" "}
                          },
                          false);



    //draw right attack slot
    y = max(0, min(LINES - 1, LINES - dock_height + 1));
    x = max(0, min(COLS - 1, COLS - orb_diameter - (dock_height - 1) * 2 - 4));

    frame = getAttackSlot(frame, y, x,
                          dock_height - 1, player.secondarySkill->icon,
                          NCOLOR_GREY, NCOLOR_BLACK);


    //draw potion slots
    y = max(0, min(LINES - 1, y + 1));
    x = max(0, min(COLS - 1, x - (dock_height - 3) * 2 * 4 - 1));
    frame = getPotionBar(frame, y, x,
                         dock_height - 3, (dock_height - 3) * 2,
                         NCOLOR_WHITE, NCOLOR_BLACK);




    //draw energy orb
    y = max(0, min(LINES - 1, LINES - (orb_diameter / 2) - 2));
    x = max(0, min(COLS - 1, COLS - orb_diameter - 3));
    int energy_level = static_cast<int>(static_cast<float>(player.current_energy) / player.max_energy * 100);

    frame = getOrb(frame, y, x,
                   orb_diameter, energy_level,
                   NCOLOR_WHITE, player.color);



    ////draw combat log
    //int log_size = min(LINES - orb_diameter - 2, static_cast<int>(combat_log.size()));
    //
    //
    //for (int i = 0; i < log_size; i++) {
    //    mvaddstr(y + i, x, combat_log[combat_log.size() - log_size + i].c_str());
    //}

    frame = getFpsLabel(frame, c_fps);


    return frame;
}
