#pragma once

#include "components/Frame.h"
#include "components/player/Player.h"
#include "constants.h"
#include "helpers.h"

using namespace std;

class UISystem {
public:
    UISystem();

    ~UISystem();

    int running;

    void CleanUp();

    void Update();


    static Frame BlankFrame(int y, int x);


    static Frame &
    addBar(Frame &frame, int y, int x, int length, float level,
           const wstring &icon, pair<int, int> color);

    static Frame &
    addButton(Frame &frame, int y, int x, int height, int width,
              pair<int, int> color);

    static Frame &
    addText(Frame &frame, int y, int x, wstring text,
            pair<int, int> color);


    Frame getFpsLabel(Frame &frame, const int c_fps);

    Frame getTitleCard(Frame frame);

    Frame getLoadCard(Frame frame);

    Frame getGameOverCard(Frame frame);

    Frame getMainMenu(Frame frame);

    Frame getNewGameMenu(Frame frame);

    Frame getLoadGameMenu(Frame frame);

    Frame getSettingsMenu(Frame frame);

    static Frame getColorDebug(Frame frame);


    Frame getInGameHud(Frame frame, const Player &player, const int c_fps);


private:
    bool Initialize();


    static Frame &
    getUiBg(Frame &frame, int dock_height,
            const pair<int, int> &ui_bg_color);

    static Frame &
    getPlayerTag(Frame &frame, int y, int x,
                 int player_level, const string &player_class,
                 int color);

    static Frame &
    getOrb(Frame &frame, int y, int x, int diameter, int level,
           pair<int, int> color);

    static Frame &
    getAttackSlot(Frame &frame, int y, int x, int height,
                  vector<vector<wstring>> icon, pair<int, int> color);

    Frame &
    getMenuButton(Frame &frame, int y, int x, int height,
                  vector<vector<wstring>> icon, bool active);

    Frame &
    getPotionBar(Frame &frame, int y, int x, int slot_height, int slot_width,
                 pair<int, int> color);
};
