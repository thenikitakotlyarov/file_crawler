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


    Frame BlankFrame(int y, int x);

    Frame getTitleCard(Frame frame);

    Frame getLoadCard(Frame frame);

    Frame getMainMenu(Frame frame);

    Frame getNewGameMenu(Frame frame);

    Frame getLoadGameMenu(Frame frame);

    Frame getSettingsMenu(Frame frame);

    Frame getColorDebug(Frame frame);

    Frame getInGameHud(Frame frame, Player player);


private:
    bool Initialize();


    Frame &
    addBar(Frame &frame, int y, int x, int length, float level,
           wstring icon, pair<int, int> color);

    Frame &
    addButton(Frame &frame, int y, int x, int height, int width,
              pair<int, int> color);

    Frame &
    addText(Frame &frame, int y, int x, wstring text,
            pair<int, int> color);


    Frame &
    getUiBg(Frame &frame, int dock_height,
            const pair<int, int> &ui_bg_color) const;

    Frame &
    getPlayerTag(Frame &frame, int y, int x,
                 int player_level, const string &player_class,
                 int color);

    Frame &
    getOrb(Frame &frame, int y, int x, int diameter, int level,
           pair<int, int> color);

    Frame &
    getAttackSlot(Frame &frame, int y, int x, int height,
                  int skill_index, pair<int, int> color);

    Frame &
    getLevelingButton(Frame &frame, int y, int x, int height,
                      bool active);

    Frame &
    getPotionBar(Frame &frame, int y, int x, int slot_height, int slot_width,
                 pair<int, int> color);
};
